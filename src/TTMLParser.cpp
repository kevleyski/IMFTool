/* Copyright(C) 2016 Björn Stresing, Denis Manthey, Wolfgang Ruppel, Krispin Weiss
 *
 * This program is free software : you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.If not, see <http://www.gnu.org/licenses/>.
 */
#include "TTMLParser.h"
#include "AS_DCP_internal.h"
#include "AS_02.h"
#include "PCMParserList.h"
#include <KM_fileio.h>
#include <cmath>
#include <QtCore>
#include <QDebug>
#include <QCryptographicHash>
#include <cstring>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

using namespace xercesc;


//!this class helps to convert char* to XMLString by using X(char*) instead of XMLString::transcode(char*).
class XStr
{
public:
	XStr(const char* const toTranscode) { fUnicodeForm = XMLString::transcode(toTranscode); }
	~XStr() { XMLString::release(&fUnicodeForm); }
	const XMLCh* unicodeForm() const { return fUnicodeForm; }

private:
	XMLCh*   fUnicodeForm;
};
#define X(str) XStr(str).unicodeForm()

elem::elem(xercesc::DOMNode *rNode, TTMLParser *rParser, elem *rParent) {

	parser = rParser;
	parent = rParent;
	node = rNode;
	el = dynamic_cast<DOMElement*>(rNode);
}

bool elem::process() {

	if (el) {

		// check if is metadata -> ignore
		if (XMLString::transcode(el->getTagName()) == "metadata") {
			dur = 2;
			stop = true;
			return stop;
		}

		// check for region attribute
		GetRegion();

		// check for timing
		if (GetStartEndTimes()) {
			is_timed = true;
			stop = true;

			processTimedElement();
		}
		else { // no timing attributes!
			GetTimeContainer(); // check for timeContainer attribute
			GetStyle();
		}
	}

	return stop;
}

void elem::GetTimeContainer() {

	// check if parent has timing -> pass on values if this is the case
	if (parent && parent->is_timed) { 
		is_timed = true;
		timeContainer = parent->timeContainer;
	}


	// check for timing attributes
	QString timeContainerVal = XMLString::transcode(el->getAttribute(XMLString::transcode("timeContainer")));
	if (timeContainerVal.length() > 0) { // found timeContainer!

		is_timed = true;

		if (timeContainerVal == "seq") {
			timeContainer = 2;
		}
		else if (timeContainerVal == "par") {
			timeContainer = 1;
		}
	}

	/*
	if (parent && parent->timeContainer == 2 && timeContainer == 2) {
		// current element and parent are seq
	}
	else if (parent && parent->timeContainer == 1 && timeContainer == 2) {
		// current element is par, parent is seq
	}
	else if (timeContainer != 0) {
		// no parent but timeContainer is set

	}
	*/

	// timing is either par or seq
	if (is_timed) {

		QString durVal = XMLString::transcode(el->getAttribute(XMLString::transcode("dur")));
		QString endVal = XMLString::transcode(el->getAttribute(XMLString::transcode("end")));

		if (durVal.length() > 0) { // dur is set!
			
			dur = ConvertTimingQStringtoDouble(durVal, parser->framerate, parser->tickrate);
			dur_set = true;

			if (parent && parent->dur_set) { // parent dur is set
				if (parent->dur_used < parent->dur) { // there is at least some duration left
					
					if (dur >= (parent->dur - parent->dur_used)) {
						dur = parent->dur - parent->dur_used; // use whatever is left over from parent dur
					}

					// check if parent is par or seq
					if (parent->timeContainer == 2) { // seq
						parent->dur_used += dur;
					} // else: parent is par

				}
				else {
					stop = true; // discontinue processing children
				}
			}
		}
		else if (parent && parent->dur_set) {
			dur_set = true;
			dur = 0;
		}

		if (endVal.length() > 0 && durVal.length() == 0) { // end is set -> use as dur!
			// use end only if dur is undefined?
			dur = ConvertTimingQStringtoDouble(endVal, parser->framerate, parser->tickrate);
			dur_set = true;
		}
	}
}

void elem::GetRegion() {

	if (parent && parent->regionName.length() > 0) { // parent element has region -> pass it on!
		regionName = parent->regionName;
		qDebug() << "parent region" << regionName;
	}
	else { // check if current element has region

		//DOMElement *el = dynamic_cast<DOMElement*>(node);
		QString regVal = XMLString::transcode(el->getAttribute(XMLString::transcode("region")));
		if (regVal.length() > 0) { // found region!
			qDebug() << "region used" << regVal;
			regionName = regVal;
		}
		else if(is_timed) {
			qDebug() << "no region found";
		}
	}
}

void elem::GetStyle() {

	// get style from parent
	if (parent && parent->css.length() > 0) {
		css.append(parent->css);
	}

	// look for style attribute
	QString styleVal = XMLString::transcode(el->getAttribute(XMLString::transcode("style")));
	if (!styleVal.isEmpty()) {
		css.append(parser->styles[styleVal]);
		el->removeAttribute(XMLString::transcode("style"));
	}

	// loop attributes
	if (el->hasAttributes()) { // loop them
		for (int z = 0; z < el->getAttributes()->getLength(); z++) {

			DOMAttr *attr = dynamic_cast<DOMAttr*>(el->getAttributes()->item(z));

			QString name = XMLString::transcode(attr->getNodeName());
			if (!parser->cssAttr[name].isEmpty()) { // known parameter!

				//if (css.count(parser->cssAttr[name]) > 0) { // css param already exists through style
				//	css.remove(QRegExp(QString("%1:[a-zA-Z0-9#]*;").arg(parser->cssAttr[name]))); // remove it
				//}

				css.append(QString("%1:%2;").arg(parser->cssAttr[name]).arg(XMLString::transcode(attr->getNodeValue())));
				//el->removeAttribute(attr->getNodeName()); // remove current attr
			}
		}
	}
}

bool elem::processTimedElement() {

	// is element between in- and -out point?
	if ((timing_begin < parser->timeline_in && timing_begin > parser->timeline_out)
		|| (timing_end < parser->timeline_in && timing_end > parser->timeline_out)) {
		return false;
	}

	// was specified parent-block duration exeeded?
	if (parent && parent->dur_set && (parent->dur_used + dur) > parent->dur) {
		return false;
	}

	// create new timed element
	TTMLelem ttml_timed;
	ttml_timed.beg = timing_begin + parser->seq_timing_total_offset;
	ttml_timed.end = timing_end + parser->seq_timing_total_offset;

	// add offset from parent (if available)
	if (parent) { 
		ttml_timed.beg += parent->dur_used;
		ttml_timed.end += parent->dur_used;
	}

	if (regionName.length() > 0 && is_timed) { // region is defined -> resolve it!

		TTMLRegion new_region;
		new_region = parser->regions[regionName]; // resolve region
		new_region.valid = true;
		ttml_timed.region = new_region;
	}

	QString image = XMLString::transcode(el->getAttribute(XMLString::transcode("smpte:backgroundImage")));
	if (!image.isEmpty()) {
		ttml_timed.type = 1; // image

		if (parser->is_wrapped) {

			AS_02::TimedText::Type5UUIDFilenameResolver NameToKumuUUID;
			// create UUID from file name e.g. track5-frag0-sample1-subs4.png -> 0c209959-84e2-5a63-86ad-bd5406b068d1
			Kumu::UUID id;// = NameToKumuUUID.NameToKumuID(image.toStdString());
			NameToKumuUUID.PngNameToType5UUID(image.toStdString(), id);
			if (!parser->anc_resources[id].isNull()) { // anc asset found!
				ttml_timed.bgImage = parser->anc_resources[id];
			}
			else { // no anc asset found -> use default
				ttml_timed.bgImage = parser->error_image; // ERROR
				ttml_timed.error = true; // set error flag
			}

			char buf[64];
			//qDebug() << "reading image" << image << id.EncodeString(buf, 64);
		}
		else {
			// look for asset in base directory
			QString dirCopyPath = QDir::currentPath();
			QDir::setCurrent(parser->baseDir.absolutePath());

			ttml_timed.bgImage = QImage(image);
			if (ttml_timed.bgImage.isNull()) {
				ttml_timed.bgImage = parser->error_image;
			}
			qDebug() << "anc image" << ttml_timed.bgImage.byteCount();
			QDir::setCurrent(dirCopyPath); // reset dir change
		}

		//ttml_div.append(ttml_timed); // add to QVector of timed elements
	}
	else { // TEXT
		ttml_timed.text = serializeTT(processTimedElementStyle(el, true));
		ttml_timed.type = 0; // set type : text
	}

	// is content valid?
	if (ttml_timed.text.length() > 0 || !ttml_timed.bgImage.isNull()) {
		parser->this_segment->items.append(ttml_timed);
	}

	
	// add visible element duration to parent block duration if parent timing is seq
	if (parent && parent->timeContainer == 2) {
		parent->dur_used += dur;
	}

	return true;
}

DOMElement* elem::processTimedElementStyle(DOMElement *rEl, bool HasTiming) {

	QString css;

	// timed element: get styles from parent
	if (HasTiming) {

		// get style from region
		if (regionName.length() > 0) {
			css.append(parser->regions[regionName].styleCss);
			qDebug() << "region" << parser->regions[regionName].styleCss;
		}

		// get style from parent
		if (parent && parent->css.length() > 0) {
			css.append(parent->css);
			qDebug() << "parent" << parent->css;
		}
	}


	// look for style attribute
	QString style = XMLString::transcode(rEl->getAttribute(XMLString::transcode("style")));
	if (!style.isEmpty()) {
		css.append(parser->styles[style]);
		rEl->removeAttribute(XMLString::transcode("style"));
	}

	// loop attributes
	if (rEl->hasAttributes()) { // loop them
		for (int z = 0; z < rEl->getAttributes()->getLength(); z++) {

			DOMAttr *attr = dynamic_cast<DOMAttr*>(rEl->getAttributes()->item(z));

			QString name = XMLString::transcode(attr->getNodeName());
			if (!parser->cssAttr[name].isEmpty()) { // known parameter!

				//if (css.count(parser->cssAttr[name]) > 0) { // css param already exists through style
					//css.remove(QRegExp(QString("%1:[a-zA-Z0-9#]*;").arg(parser->cssAttr[name]))); // remove it
				//}

				css.append(QString("%1:%2;").arg(parser->cssAttr[name]).arg(XMLString::transcode(attr->getNodeValue())));
				//el->removeAttribute(attr->getNodeName()); // remove current attr
			}
			else {
				//qDebug() << "unknown attr" << name << XMLString::transcode(attr->getNodeValue());
			}
		}
	}

	if (css.length() > 0) {
		rEl->setAttribute(XMLString::transcode("style"), XMLString::transcode(css.toStdString().c_str()));
	}

	// loop children (if available...)
	if (rEl->getChildNodes()) {
		for (int i = 0; i < rEl->getChildNodes()->getLength(); i++) {
			DOMElement* child = dynamic_cast<DOMElement*>(rEl->getChildNodes()->item(i)->cloneNode(true));
			if (child) {
				DOMNode *processed_child = dynamic_cast<DOMNode*>(processTimedElementStyle(child, false));
				rEl->replaceChild(processed_child, rEl->getChildNodes()->item(i));
			}
		}
	}

	return rEl;
}

bool elem::GetStartEndTimes() {

	float duration = 0, end = 0, beg = 0, current_dur = 0;
	bool end_found = false, beg_found = false, dur_found = false;

	if (!el) return false;

	QString tc_string = XMLString::transcode(el->getAttribute(XMLString::transcode("timeContainer")));
	if (!tc_string.isEmpty()) {
		return false; // abort!
	}

	QString end_string = XMLString::transcode(el->getAttribute(XMLString::transcode("end")));
	if (!end_string.isEmpty()) {
		end = ConvertTimingQStringtoDouble(end_string, parser->framerate, parser->tickrate);
		end_found = true;
	}

	QString beg_string = XMLString::transcode(el->getAttribute(XMLString::transcode("begin")));
	if (!beg_string.isEmpty()) {
		beg = ConvertTimingQStringtoDouble(beg_string, parser->framerate, parser->tickrate);
		beg_found = true;
	}

	QString dur_string = XMLString::transcode(el->getAttribute(XMLString::transcode("dur")));
	if (!dur_string.isEmpty()) {
		current_dur = ConvertTimingQStringtoDouble(dur_string, parser->framerate, parser->tickrate);
		dur_found = true;
	}

	if (end_string.isEmpty()) {
		end = beg + current_dur;
	}

	dur = end;

	if (dur_found || (beg_found && end_found) || (beg_found && dur_found)) {
		hasBegEndDur = true;
		timing_begin = beg;
		timing_end = end;
		timing_dur = end - beg;

		// dur
		if (parent && (parent->dur_set || parent->end_set)) {
			//parent->dur_used += end; // add current duration to total duration used in the block
		}

		return true;
	}
	else if(dur_found || beg_found || end_found){ // invisible element
		hasBegEndDur = true;
		// dur
		if (parent && (parent->dur_set || parent->end_set)) {
			parent->dur_used += end; // add current duration to total duration used in the block
		}
	}

	return false;
}

QString elem::serializeTT(DOMElement *rEl) {

	//Return the first registered implementation that has the desired features. In this case, we are after a DOM implementation that has the LS feature... or Load/Save. 
	DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(X("LS"));

	// Create a DOMLSSerializer which is used to serialize a DOM tree into an XML document. 
	DOMLSSerializer *serializer = ((DOMImplementationLS*)implementation)->createLSSerializer();

	return QString(XMLString::transcode(serializer->writeToString(rEl)));
}


// --------------------------------------------------------------------------------------------

TTMLParser::TTMLParser() {
	
	// initialize values
	cssAttr["tts:backgroundColor"] = "background-color";
	cssAttr["tts:fontSize"] = "font-size";
	cssAttr["tts:fontFamily"] = "font-family";
	cssAttr["tts:fontStyle"] = "font-style";
	cssAttr["tts:textDecoration"] = "text-decoration";
	cssAttr["tts:textAlign"] = "text-align";
	cssAttr["tts:fontWeight"] = "font-weight";
	cssAttr["tts:color"] = "color";
	cssAttr["tts:opacity"] = "opacity";
	cssAttr["tts:direction"] = "direction";
	cssAttr["tts:unicode-bidi"] = "unicode-bidi";
	cssAttr["tts:padding"] = "padding";

	error_image = QImage(":/ttml_bg_not_found.png");
	nullregion.valid = false;

	region_colors.append(QColor(120, 0, 244, 100)); // color 0
	region_colors.append(QColor(0, 244, 120, 100)); // color 1
	region_colors.append(QColor(255, 200, 0, 100)); // color 2
	region_colors.append(QColor(100, 244, 0, 100)); // color 3
}

Error TTMLParser::open(const QString &rSourceFile, TTMLtimelineSegment &ttml_segment, bool rIsWrapped) {

	Error error;
	
	this_segment = &ttml_segment;
	timeline_in = ttml_segment.in;
	timeline_out = ttml_segment.out;
	is_wrapped = rIsWrapped;
	baseDir = QFileInfo(rSourceFile).absoluteDir();
	SourceFilePath = rSourceFile;

	TimedText::FrameBuffer buff(2 * Kumu::Megabyte);
	AS_02::TimedText::TimedTextDescriptor TDesc;
	std::string XMLDoc;

	if (is_wrapped) {
		AS_02::Result_t result = reader.OpenRead(rSourceFile.toStdString()); // "D:/Master/Thesis/TTML/track5-frag2-sample1-subs0.mxf"
		if (ASDCP_SUCCESS(result)) {

			result = reader.ReadTimedTextResource(XMLDoc); // read TTML XML
			if (ASDCP_SUCCESS(result)) {
				Save2File(XMLDoc);
				readAncilleryData();
				parse(XMLDoc);
			}
		}
		else {
			error = Error(result);
			return error;
		}
	}
	else { // read no-yet-wrapped .ttml file
		parse(XMLDoc); // XMLDoc : empty
	}

	return error;
}


void TTMLParser::readAncilleryData() {

	// get ancillary data
	AS_02::TimedText::TimedTextDescriptor TDesc;
	AS_02::Result_t result = reader.FillTimedTextDescriptor(TDesc);
	TimedText::FrameBuffer buffer;
	buffer.Capacity(2 * Kumu::Megabyte);
	
	if (ASDCP_SUCCESS(result)) {
		AS_02::TimedText::ResourceList_t::const_iterator ri;

		for (ri = TDesc.ResourceList.begin(); ri != TDesc.ResourceList.end() && ASDCP_SUCCESS(result); ri++) {

			Kumu::UUID id;
			id.Set((*ri).ResourceID);

			result = reader.ReadAncillaryResource((*ri).ResourceID, buffer);

			if (ASDCP_SUCCESS(result)) {
				qDebug() << "successfully read anc data";
				QPixmap pix;
				pix.loadFromData(buffer.RoData(), buffer.Size(), "png");
				anc_resources[id] = pix.toImage();
			}
			else {
				qDebug() << "error reading anc";
			}
		}
	}
	else {
		qDebug() << "no resource list found";
	}
}

void TTMLParser::parse(std::string xml) {

	// SETUP XERCES
	XercesDOMParser*   parser = NULL;
	ErrorHandler*      errorHandler = NULL;

	if (!parser)
	{
		XMLPlatformUtils::Initialize();
		parser = new XercesDOMParser();
	}
	
	if (is_wrapped) {
		xercesc::MemBufInputSource memBuffInput((const XMLByte*)xml.c_str(), xml.size(), "dummy", false);
		parser->parse(memBuffInput);
	}
	else {
		try {
			parser->parse(SourceFilePath.toLocal8Bit());
		}
		catch (...) {
			qDebug() << "ERROR parsing" << SourceFilePath;
		}
	}
	
	DOMDocument *dom_doc = parser->getDocument();															

	// get metadata
	getMetadata(dom_doc);

	// set framerate in TTMLtimelineSegment
	this_segment->frameRate = framerate;

	// loop styles
	DOMNodeList	*styleList = dom_doc->getElementsByTagName(XMLString::transcode("style"));
	for (int i = 0; i < styleList->getLength(); i++) {
		QVector<QString> style = attrs2Css(styleList->item(i));
		styles[style[0]] = style[1]; // e.g. ["white_8"] = "CSS"
	}
	
	// loop regions
	DOMNodeList	*regionList = dom_doc->getElementsByTagName(XMLString::transcode("region"));
	for (int i = 0; i < regionList->getLength(); i++) {
		TTMLRegion region = parseRegion(regionList->item(i));
		regions[ region.id ] = region; // e.g. ["region1"] = TTMLRegion
		qDebug() << "created region" << region.id;
	}

	// loop all children of <body>
	DOMNodeList	*body = dom_doc->getElementsByTagName(XMLString::transcode("body"))->item(0)->getChildNodes();
	for (int i = 0; i < body->getLength(); i++) {
		
		elem *newEl = new elem(body->item(i), this, NULL); // pass current parser & parent element
		if (newEl->process() == false) { // continue processing children
			elems.append(newEl);
			RloopElements(newEl);
		}
		else {
			delete newEl; // delete instance
		}
	}

	print2Console(this_segment->items);
	parser->~XercesDOMParser(); // delete DOM parser
}

TTMLRegion TTMLParser::parseRegion(DOMNode *node) {

	TTMLRegion region;

	DOMElement *el = dynamic_cast<DOMElement*>(node);

	// get id
	QString idVal = XMLString::transcode(el->getAttribute(XMLString::transcode("xml:id")));
	if (!idVal.isEmpty()) { // found id
		region.id = idVal;
		el->removeAttribute(XMLString::transcode("xml:id"));
	}

	// get style
	QString styleVal = XMLString::transcode(el->getAttribute(XMLString::transcode("style")));
	if (!styleVal.isEmpty()) { // found style
		region.styleCss.append(styles[styleVal]);
		el->removeAttribute(XMLString::transcode("style"));
	}

	// get origin e.g. tts:origin='12.88% 0.417%'
	QString originVal = XMLString::transcode(el->getAttribute(XMLString::transcode("tts:origin")));
	if (!originVal.isEmpty()) { // found origin
		if (originVal.count("%") > 0) { // %
			originVal.replace("%", ""); // remove '%'
			QStringList values = originVal.split(" ");
			region.origin[0] = values[0].toFloat();
			region.origin[1] = values[1].toFloat();
		}
		else { // px
			originVal.replace("px", ""); // remove 'px'
			QStringList values = originVal.split(" ");
			region.origin[0] = (values[0].toFloat() / extent[0]) * 100;
			region.origin[1] = (values[1].toFloat() / extent[1]) * 100;
		}
		el->removeAttribute(XMLString::transcode("tts:origin")); // remove attr
	}

	// get extent e.g. tts:extent='580px 200px'
	QString extentVal = XMLString::transcode(el->getAttribute(XMLString::transcode("tts:extent")));
	if (!extentVal.isEmpty()) { // found extent
		if (extentVal.count("%") > 0) { // %
			extentVal.replace("%", ""); // remove '%'
			QStringList values = extentVal.split(" ");
			region.extent[0] = values[0].toFloat();
			region.extent[1] = values[1].toFloat();
		}
		else { // px
			extentVal.replace("px", ""); // remove 'px'
			QStringList values = extentVal.split(" ");
			region.extent[0] = (values[0].toFloat() / extent[0]) * 100;
			region.extent[1] = (values[1].toFloat() / extent[1]) * 100;
		}
		el->removeAttribute(XMLString::transcode("tts:extent")); // remove attr
	}

	// check when region is active
	QString showVal = XMLString::transcode(el->getAttribute(XMLString::transcode("tts:showBackground")));
	if (!showVal.isEmpty()) { // found showBackground
		if (showVal == "always") {
			region.alwaysActive = true;
		}
		else {
			region.alwaysActive = false;
		}
		el->removeAttribute(XMLString::transcode("tts:showBackground")); // remove attr
	}

	// loop remaining attributes
	if (el->hasAttributes()) { // loop them
		for (int z = 0; z < el->getAttributes()->getLength(); z++) {

			DOMAttr *attr = dynamic_cast<DOMAttr*>(el->getAttributes()->item(z));

			QString name = XMLString::transcode(attr->getNodeName());
			if (!cssAttr[name].isEmpty()) { // known parameter!

				//if (region.styleCss.count(cssAttr[name]) > 0) { // css param already exists through style
					//region.styleCss.remove(QRegExp(QString("%1:[a-zA-Z0-9#]*;").arg(cssAttr[name]))); // remove it
				//}

				region.styleCss.append(QString("%1:%2;").arg(cssAttr[name]).arg(XMLString::transcode(attr->getNodeValue())));
				//el->removeAttribute(attr->getNodeName()); // remove current attr
			}
			else {
				qDebug() << "unknown attr" << name << XMLString::transcode(attr->getNodeValue());
			}
		}
	}

	qDebug() << "region id" << region.id;
	// check if region has a backround-color
	if (region.styleCss.count("background-color") > 0) {
		// use it
		QRegExp rx("background-color:([a-zA-Z0-9#]*);");
		int pos = rx.indexIn(region.styleCss);
		QStringList list = rx.capturedTexts();
		QString col = list[0].replace("background-color:", "");
		col.replace(";", "");
		region.bgColor.setNamedColor(col); // orange, #ff8000, #ff6 ...
		region.bgColor.setAlpha(200);
		qDebug() << "region color" << col << region.styleCss;
	}
	else {
		// assign one
		region.bgColor = QColor(120, 0, 244, 100); // default color
	}

	return region;
}


QVector<QString> TTMLParser::attrs2Css(DOMNode *el) {

	QString css;
	QVector<QString> ret(2);
	// get attributes
	if (el->hasAttributes()) { // loop them
		for (int z = 0; z < el->getAttributes()->getLength(); z++) {

			DOMNode *attr = el->getAttributes()->item(z);

			QString name = XMLString::transcode(attr->getNodeName());
			if (name == "xml:id") {
				ret[0] = XMLString::transcode(attr->getNodeValue());
			}else if (!cssAttr[name].isEmpty()) { // known parameter!
				css.append(QString("%1:%2;").arg(cssAttr[name]).arg(XMLString::transcode(attr->getNodeValue())));
			}
		}
	}

	ret[1] = css;
	return ret;
}

void TTMLParser::RloopElements(elem *el) {


	if (el->node) {

		DOMNodeList *children = el->node->getChildNodes();

		// recursively loop children
		for (int z = 0; z < children->getLength(); z++) {

			elem *newEl = new elem(children->item(z), this, el); // pass current parser & parent element
			if (newEl->process() == false) { // continue one step deeper
				elems.append(newEl);
				RloopElements(newEl);

				if (newEl->parent->timeContainer == 2 && newEl->hasBegEndDur == false) {
					seq_timing_total_offset += newEl->dur; // add block duration (if sequential) to total offset
				}
			}
			else {
				delete newEl; // destroy elem instance
			}
		}
	}
}

// Code by Denis Manthey
void TTMLParser::getMetadata(DOMDocument *rDom) {

	//Frame Rate Multiplier Extractor
	DOMNodeList *multitem = rDom->getElementsByTagName(XMLString::transcode("tt"));
	DOMElement* multeleDom = dynamic_cast<DOMElement*>(multitem->item(0));
	QString mult = XMLString::transcode(multeleDom->getAttribute(XMLString::transcode("ttp:frameRateMultiplier")));
	float num = 1000;
	float den = 1000;
	if (!mult.isEmpty()) {
		num = mult.left(4).toInt();
		den = mult.right(4).toInt();
	}

	//Frame Rate Extractor
	DOMNodeList *fritem = rDom->getElementsByTagName(XMLString::transcode("tt"));
	DOMElement* freleDom = dynamic_cast<DOMElement*>(fritem->item(0));
	QString fr = XMLString::transcode(freleDom->getAttribute(XMLString::transcode("ttp:frameRate")));
	int editrate = 30;					//editrate is for the metadata object (expects editrate*numerator, denominator)
	framerate = 30 * (num / den);		//framerate is for calculating the duration, we need the fractal editrate!
	if (!fr.isEmpty()) {
		framerate = fr.toFloat()*(num / den);
		editrate = fr.toInt();
	}

	//Tick Rate Extractor
	DOMNodeList *tritem = rDom->getElementsByTagName(XMLString::transcode("tt"));
	DOMElement* treleDom = dynamic_cast<DOMElement*>(tritem->item(0));
	QString tr = XMLString::transcode(treleDom->getAttribute(XMLString::transcode("ttp:tickRate")));
	tickrate = tr.toInt();

	//Duration Extractor
	float duration;
	duration = DurationExtractor(rDom, framerate, tickrate);
	if (duration == 0) {
		qDebug() << "Unknown duration!";
	}

	// get extent (e.g. tts:extent='854px 480px')
	QString extentVal = XMLString::transcode(multeleDom->getAttribute(XMLString::transcode("tts:extent")));
	if (!extentVal.isEmpty()) {
		if (extentVal.count("px") > 0) {
			extentVal.replace("px", ""); // remove 'px'
			QStringList values = extentVal.split(" ");
			extent[0] = values[0].toFloat();
			extent[1] = values[1].toFloat();
		}
	}
}


void TTMLParser::Save2File(std::string xml) {

	QString filename = "D:/Master/Thesis/test.txt";
	QFile file(filename);
	if (file.open(QIODevice::ReadWrite))
	{
		QTextStream stream(&file);
		stream << xml.data() << endl;
		qDebug() << "XML saved!";
	}
}

void TTMLParser::print2Console(const QVector<TTMLelem> &ttmls) {

	qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~ START printing TTML ~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	
	for (int i = 0; i < ttmls.length(); i++) {

		if (ttmls[i].type == 0) { // text
			qDebug() << ttmls[i].beg << " -> " << ttmls[i].end << "TEXT:" << ttmls[i].text;
		}
		else if(ttmls[i].type == 1){ // image
			qDebug() << ttmls[i].beg << " -> " << ttmls[i].end << "IMAGE:" << ttmls[i].bgImage.byteCount();
		}
		else { // unknown
			qDebug() << "ERROR: unknown type";
		}
	}

	qDebug() << "~~~~~~~~~~~~~~~~~~~~~~~~~ END printing TTML ~~~~~~~~~~~~~~~~~~~~~~~~~~~";
}

// ###################################### functions
// Code by Denis Manthey

float TTMLFns::DurationExtractor(DOMDocument *dom_doc, float fr, int tr) {

	float eleduration = 0, divduration = 0, div2duration = 0, pduration = 0, p2duration = 0, spanduration = 0, duration = 0;

	DOMNodeList	*bodyitems = dom_doc->getElementsByTagName(XMLString::transcode("body"));
	DOMElement* bodyeleDom = dynamic_cast<DOMElement*>(bodyitems->item(0));
	QString bodytime = XMLString::transcode(bodyeleDom->getAttribute(XMLString::transcode("timeContainer")));  //bodytime: par/seq

																											   //---start with div childelements of body---
	DOMNodeList	*divitems = dom_doc->getElementsByTagName(XMLString::transcode("div"));
	divduration = 0;
	for (int i = 0; i < divitems->getLength(); i++) {

		DOMElement* diveleDom = dynamic_cast<DOMElement*>(divitems->item(i));

		QString comp = XMLString::transcode(diveleDom->getParentNode()->getNodeName());			//look for body-child div!
		if (comp != "div") {
			QString divtime = XMLString::transcode(diveleDom->getAttribute(XMLString::transcode("timeContainer")));	//divtime: par/seq
			eleduration = GetElementDuration(diveleDom, fr, tr);
			div2duration = 0;
			pduration = 0;

			if (eleduration == 0) {
				DOMNodeList *div2items = diveleDom->getElementsByTagName(XMLString::transcode("div"));
				div2duration = 0;
				for (int j = 0; j < div2items->getLength(); j++) {

					DOMElement* div2eleDom = dynamic_cast<DOMElement*>(div2items->item(j));
					QString div2time = XMLString::transcode(div2eleDom->getAttribute(XMLString::transcode("timeContainer"))); //div2time: par/seq

					eleduration = GetElementDuration(div2eleDom, fr, tr);
					p2duration = 0;

					if (eleduration == 0) {
						DOMNodeList *p2items = div2eleDom->getElementsByTagName(XMLString::transcode("p"));
						p2duration = 0;
						for (int k = 0; k < p2items->getLength(); k++) {

							DOMElement* p2eleDom = dynamic_cast<DOMElement*>(p2items->item(k));
							QString p2time = XMLString::transcode(p2eleDom->getAttribute(XMLString::transcode("timeContainer"))); //p2time: par/seq

							eleduration = GetElementDuration(p2eleDom, fr, tr);
							spanduration = 0;

							if (eleduration == 0) {
								DOMNodeList *spanitems = p2eleDom->getElementsByTagName(XMLString::transcode("span"));

								for (int l = 0; l < spanitems->getLength(); l++) {

									DOMElement* spaneleDom = dynamic_cast<DOMElement*>(spanitems->item(l));

									eleduration = GetElementDuration(spaneleDom, fr, tr);
									if (p2time == "seq")
										spanduration = spanduration + eleduration;
									else {
										if (eleduration > spanduration)
											spanduration = eleduration;
									}
								}
								eleduration = 0;
							}

							if (div2time == "seq")
								p2duration = p2duration + eleduration + spanduration;
							else {
								if (spanduration > p2duration)
									p2duration = spanduration;

								if (eleduration > p2duration)
									p2duration = eleduration;
							}
						}
						eleduration = 0;
					}

					if (divtime == "seq")
						div2duration = div2duration + eleduration + p2duration;
					else {
						if (p2duration > div2duration)
							div2duration = p2duration;

						if (eleduration > div2duration)
							div2duration = eleduration;
					}
				}
				eleduration = 0;

				DOMNodeList *pitems = diveleDom->getElementsByTagName(XMLString::transcode("p"));
				for (int m = 0; m < pitems->getLength(); m++) {
					DOMElement* peleDom = dynamic_cast<DOMElement*>(pitems->item(m));
					QString comp2 = XMLString::transcode(peleDom->getParentNode()->getParentNode()->getNodeName());
					if (comp2 == "body") {
						QString ptime = XMLString::transcode(peleDom->getAttribute(XMLString::transcode("timeContainer")));	//ptime: par/seq
						eleduration = GetElementDuration(peleDom, fr, tr);
						spanduration = 0;

						if (eleduration == 0) {
							DOMNodeList *spanitems = peleDom->getElementsByTagName(XMLString::transcode("span"));

							for (int n = 0; n < spanitems->getLength(); n++) {

								DOMElement* spaneleDom = dynamic_cast<DOMElement*>(spanitems->item(n));

								eleduration = GetElementDuration(spaneleDom, fr, tr);
								if (ptime == "seq")
									spanduration = spanduration + eleduration;
								else {
									if (eleduration > spanduration)
										spanduration = eleduration;
								}
							}
							eleduration = 0;
						}

						if (divtime == "seq")
							pduration = pduration + eleduration + spanduration;
						else {
							if (spanduration > pduration)
								pduration = spanduration;

							if (eleduration > pduration)
								pduration = eleduration;
						}
					}
					eleduration = 0;
				}

				if (divtime == "seq")
					divduration = divduration + pduration + div2duration;
				else {
					if (div2duration > pduration)
						divduration = div2duration;
					else
						divduration = pduration;
				}
			}

			if (bodytime == "seq")
				duration = duration + eleduration + divduration;
			else {
				if (divduration > eleduration)
					duration = divduration;

				else
					duration = eleduration;
			}
		}
	}
	return duration;
}

float TTMLFns::GetElementDuration(DOMElement* eleDom, float fr, int tr) {

	float duration = 0, end = 0, beg = 0, dur = 0;
	QString end_string = XMLString::transcode(eleDom->getAttribute(XMLString::transcode("end")));
	if (!end_string.isEmpty())
		end = ConvertTimingQStringtoDouble(end_string, fr, tr);

	QString beg_string = XMLString::transcode(eleDom->getAttribute(XMLString::transcode("begin")));
	if (!beg_string.isEmpty())
		beg = ConvertTimingQStringtoDouble(beg_string, fr, tr);

	QString dur_string = XMLString::transcode(eleDom->getAttribute(XMLString::transcode("dur")));
	if (!dur_string.isEmpty())
		dur = ConvertTimingQStringtoDouble(dur_string, fr, tr);

	if (!end_string.isEmpty())
		duration = end;
	else
		duration = beg + dur;

	if (dur == 0 && end == 0)
		duration = 0;

	return duration;
}

float TTMLFns::ConvertTimingQStringtoDouble(QString string_time, float fr, int tr) {

	float time, h, min, sec, msec;

	if (string_time.right(2) == "ms")
		time = string_time.remove(QChar('m'), Qt::CaseInsensitive).remove(QChar('s'), Qt::CaseInsensitive).toFloat() / 1000;

	else if (string_time.right(1) == "s")
		time = string_time.remove(QChar('s'), Qt::CaseInsensitive).toFloat();

	else if (string_time.right(1) == "m")
		time = string_time.remove(QChar('m'), Qt::CaseInsensitive).toFloat() * 60;

	else if (string_time.right(1) == "h")
		time = string_time.remove(QChar('h'), Qt::CaseInsensitive).toFloat() * 3600;

	else if (string_time.right(1) == "f")
		time = string_time.remove(QChar('f'), Qt::CaseInsensitive).toFloat() / fr;

	else if (string_time.right(1) == "t")
		time = string_time.remove(QChar('t'), Qt::CaseInsensitive).toFloat() / tr;

	else if (string_time.left(9).right(1) == ".") {
		h = string_time.left(2).toFloat();
		min = string_time.left(5).right(2).toFloat();
		sec = string_time.left(8).right(2).toFloat();
		msec = string_time.remove(0, 7).replace(0, 2, "0.").toFloat();
		time = (h * 60 * 60) + (min * 60) + sec + msec;
	}

	else {
		if (tr > 0) {
			h = string_time.left(2).toFloat();
			min = string_time.left(5).right(2).toFloat();
			sec = string_time.left(8).right(2).toFloat() + (string_time.remove(0, 9).toFloat() / tr);
			time = (h * 60 * 60) + (min * 60) + sec;
		}
		else {
			h = string_time.left(2).toFloat();
			min = string_time.left(5).right(2).toFloat();
			sec = string_time.left(8).right(2).toFloat() + (string_time.remove(0, 9).toFloat() / fr);
			time = (h * 60 * 60) + (min * 60) + sec;
		}
	}
	return time;
}