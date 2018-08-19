// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

#ifndef SMPTE_2067_9A_2018_SIDECAR_H
#define SMPTE_2067_9A_2018_SIDECAR_H

#ifndef XSD_USE_CHAR
#define XSD_USE_CHAR
#endif

#ifndef XSD_CXX_TREE_USE_CHAR
#define XSD_CXX_TREE_USE_CHAR
#endif

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/config.hxx>

#if (XSD_INT_VERSION != 4000000L)
#error XSD runtime version mismatch
#endif

#include <xsd/cxx/pre.hxx>

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/types.hxx>

#include <xsd/cxx/xml/error-handler.hxx>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include <xsd/cxx/tree/parsing.hxx>
#include <xsd/cxx/tree/parsing/byte.hxx>
#include <xsd/cxx/tree/parsing/unsigned-byte.hxx>
#include <xsd/cxx/tree/parsing/short.hxx>
#include <xsd/cxx/tree/parsing/unsigned-short.hxx>
#include <xsd/cxx/tree/parsing/int.hxx>
#include <xsd/cxx/tree/parsing/unsigned-int.hxx>
#include <xsd/cxx/tree/parsing/long.hxx>
#include <xsd/cxx/tree/parsing/unsigned-long.hxx>
#include <xsd/cxx/tree/parsing/boolean.hxx>
#include <xsd/cxx/tree/parsing/float.hxx>
#include <xsd/cxx/tree/parsing/double.hxx>
#include <xsd/cxx/tree/parsing/decimal.hxx>

#include <xsd/cxx/xml/dom/serialization-header.hxx>
#include <xsd/cxx/tree/serialization.hxx>
#include <xsd/cxx/tree/serialization/byte.hxx>
#include <xsd/cxx/tree/serialization/unsigned-byte.hxx>
#include <xsd/cxx/tree/serialization/short.hxx>
#include <xsd/cxx/tree/serialization/unsigned-short.hxx>
#include <xsd/cxx/tree/serialization/int.hxx>
#include <xsd/cxx/tree/serialization/unsigned-int.hxx>
#include <xsd/cxx/tree/serialization/long.hxx>
#include <xsd/cxx/tree/serialization/unsigned-long.hxx>
#include <xsd/cxx/tree/serialization/boolean.hxx>
#include <xsd/cxx/tree/serialization/float.hxx>
#include <xsd/cxx/tree/serialization/double.hxx>
#include <xsd/cxx/tree/serialization/decimal.hxx>

#include <xsd/cxx/tree/std-ostream-operators.hxx>

namespace xml_schema
{
  // anyType and anySimpleType.
  //
  typedef ::xsd::cxx::tree::type Type;
  typedef ::xsd::cxx::tree::simple_type< char, Type > SimpleType;
  typedef ::xsd::cxx::tree::type Container;

  // 8-bit
  //
  typedef signed char Byte;
  typedef unsigned char UnsignedByte;

  // 16-bit
  //
  typedef short Short;
  typedef unsigned short UnsignedShort;

  // 32-bit
  //
  typedef int Int;
  typedef unsigned int UnsignedInt;

  // 64-bit
  //
  typedef long long Long;
  typedef unsigned long long UnsignedLong;

  // Supposed to be arbitrary-length integral types.
  //
  typedef long long Integer;
  typedef long long NonPositiveInteger;
  typedef unsigned long long NonNegativeInteger;
  typedef unsigned long long PositiveInteger;
  typedef long long NegativeInteger;

  // Boolean.
  //
  typedef bool Boolean;

  // Floating-point types.
  //
  typedef float Float;
  typedef double Double;
  typedef double Decimal;

  // String types.
  //
  typedef ::xsd::cxx::tree::string< char, SimpleType > String;
  typedef ::xsd::cxx::tree::normalized_string< char, String > NormalizedString;
  typedef ::xsd::cxx::tree::token< char, NormalizedString > Token;
  typedef ::xsd::cxx::tree::name< char, Token > Name;
  typedef ::xsd::cxx::tree::nmtoken< char, Token > Nmtoken;
  typedef ::xsd::cxx::tree::nmtokens< char, SimpleType, Nmtoken > Nmtokens;
  typedef ::xsd::cxx::tree::ncname< char, Name > Ncname;
  typedef ::xsd::cxx::tree::language< char, Token > Language;

  // ID/IDREF.
  //
  typedef ::xsd::cxx::tree::id< char, Ncname > Id;
  typedef ::xsd::cxx::tree::idref< char, Ncname, Type > Idref;
  typedef ::xsd::cxx::tree::idrefs< char, SimpleType, Idref > Idrefs;

  // URI.
  //
  typedef ::xsd::cxx::tree::uri< char, SimpleType > Uri;

  // Qualified name.
  //
  typedef ::xsd::cxx::tree::qname< char, SimpleType, Uri, Ncname > Qname;

  // Binary.
  //
  typedef ::xsd::cxx::tree::buffer< char > Buffer;
  typedef ::xsd::cxx::tree::base64_binary< char, SimpleType > Base64Binary;
  typedef ::xsd::cxx::tree::hex_binary< char, SimpleType > HexBinary;

  // Date/time.
  //
  typedef ::xsd::cxx::tree::time_zone TimeZone;
  typedef ::xsd::cxx::tree::date< char, SimpleType > Date;
  typedef ::xsd::cxx::tree::date_time< char, SimpleType > DateTime;
  typedef ::xsd::cxx::tree::duration< char, SimpleType > Duration;
  typedef ::xsd::cxx::tree::gday< char, SimpleType > Gday;
  typedef ::xsd::cxx::tree::gmonth< char, SimpleType > Gmonth;
  typedef ::xsd::cxx::tree::gmonth_day< char, SimpleType > GmonthDay;
  typedef ::xsd::cxx::tree::gyear< char, SimpleType > Gyear;
  typedef ::xsd::cxx::tree::gyear_month< char, SimpleType > GyearMonth;
  typedef ::xsd::cxx::tree::time< char, SimpleType > Time;

  // Entity.
  //
  typedef ::xsd::cxx::tree::entity< char, Ncname > Entity;
  typedef ::xsd::cxx::tree::entities< char, SimpleType, Entity > Entities;

  typedef ::xsd::cxx::tree::content_order ContentOrder;
  // Namespace information and list stream. Used in
  // serialization functions.
  //
  typedef ::xsd::cxx::xml::dom::namespace_info< char > NamespaceInfo;
  typedef ::xsd::cxx::xml::dom::namespace_infomap< char > NamespaceInfomap;
  typedef ::xsd::cxx::tree::list_stream< char > ListStream;
  typedef ::xsd::cxx::tree::as_double< Double > AsDouble;
  typedef ::xsd::cxx::tree::as_decimal< Decimal > AsDecimal;
  typedef ::xsd::cxx::tree::facet Facet;

  // Flags and properties.
  //
  typedef ::xsd::cxx::tree::flags Flags;
  typedef ::xsd::cxx::tree::properties< char > Properties;

  // Parsing/serialization diagnostics.
  //
  typedef ::xsd::cxx::tree::severity Severity;
  typedef ::xsd::cxx::tree::error< char > Error;
  typedef ::xsd::cxx::tree::diagnostics< char > Diagnostics;

  // Exceptions.
  //
  typedef ::xsd::cxx::tree::exception< char > Exception;
  typedef ::xsd::cxx::tree::bounds< char > Bounds;
  typedef ::xsd::cxx::tree::duplicate_id< char > DuplicateId;
  typedef ::xsd::cxx::tree::parsing< char > Parsing;
  typedef ::xsd::cxx::tree::expected_element< char > ExpectedElement;
  typedef ::xsd::cxx::tree::unexpected_element< char > UnexpectedElement;
  typedef ::xsd::cxx::tree::expected_attribute< char > ExpectedAttribute;
  typedef ::xsd::cxx::tree::unexpected_enumerator< char > UnexpectedEnumerator;
  typedef ::xsd::cxx::tree::expected_text_content< char > ExpectedTextContent;
  typedef ::xsd::cxx::tree::no_prefix_mapping< char > NoPrefixMapping;
  typedef ::xsd::cxx::tree::serialization< char > Serialization;

  // Error handler callback interface.
  //
  typedef ::xsd::cxx::xml::error_handler< char > ErrorHandler;

  // DOM interaction.
  //
  namespace dom
  {
    // Automatic pointer for DOMDocument.
    //
    using ::xsd::cxx::xml::dom::auto_ptr;

#ifndef XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
#define XSD_CXX_TREE_TREE_NODE_KEY__XML_SCHEMA
    // DOM user data key for back pointers to tree nodes.
    //
    const XMLCh* const treeNodeKey = ::xsd::cxx::tree::user_data_keys::node;
#endif
  }
}

// Forward declarations.
//
namespace scm
{
  class SidecarCompositionMapType;
  class SidecarAssetType;
  class SidecarCompositionMapType_PropertiesType;
  class SidecarCompositionMapType_SidecarAssetListType;
  class SidecarAssetType_AssociatedCPLListType;
}


#include <memory>    // ::std::auto_ptr
#include <limits>    // std::numeric_limits
#include <algorithm> // std::binary_search

#include <xsd/cxx/xml/char-utf8.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/elements.hxx>
#include <xsd/cxx/tree/containers.hxx>
#include <xsd/cxx/tree/list.hxx>

#include <xsd/cxx/xml/dom/parsing-header.hxx>

#include <xsd/cxx/tree/containers-wildcard.hxx>

#include "dcmlTypes.h"

#include "xmldsig-core-schema.h"

namespace scm
{
  class SidecarCompositionMapType: public ::xml_schema::Type
  {
    public:
    // Id
    //
    typedef ::dcml::UUIDType IdType;
    typedef ::xsd::cxx::tree::traits< IdType, char > IdTraits;

    const IdType&
    getId () const;

    IdType&
    getId ();

    void
    setId (const IdType& x);

    void
    setId (::std::auto_ptr< IdType > p);

    // Properties
    //
    typedef ::scm::SidecarCompositionMapType_PropertiesType PropertiesType;
    typedef ::xsd::cxx::tree::traits< PropertiesType, char > PropertiesTraits;

    const PropertiesType&
    getProperties () const;

    PropertiesType&
    getProperties ();

    void
    setProperties (const PropertiesType& x);

    void
    setProperties (::std::auto_ptr< PropertiesType > p);

    // SidecarAssetList
    //
    typedef ::scm::SidecarCompositionMapType_SidecarAssetListType SidecarAssetListType;
    typedef ::xsd::cxx::tree::traits< SidecarAssetListType, char > SidecarAssetListTraits;

    const SidecarAssetListType&
    getSidecarAssetList () const;

    SidecarAssetListType&
    getSidecarAssetList ();

    void
    setSidecarAssetList (const SidecarAssetListType& x);

    void
    setSidecarAssetList (::std::auto_ptr< SidecarAssetListType > p);

    // Signer
    //
    typedef ::ds::KeyInfoType SignerType;
    typedef ::xsd::cxx::tree::optional< SignerType > SignerOptional;
    typedef ::xsd::cxx::tree::traits< SignerType, char > SignerTraits;

    const SignerOptional&
    getSigner () const;

    SignerOptional&
    getSigner ();

    void
    setSigner (const SignerType& x);

    void
    setSigner (const SignerOptional& x);

    void
    setSigner (::std::auto_ptr< SignerType > p);

    // Signature
    //
    typedef ::ds::SignatureType SignatureType;
    typedef ::xsd::cxx::tree::optional< SignatureType > SignatureOptional;
    typedef ::xsd::cxx::tree::traits< SignatureType, char > SignatureTraits;

    const SignatureOptional&
    getSignature () const;

    SignatureOptional&
    getSignature ();

    void
    setSignature (const SignatureType& x);

    void
    setSignature (const SignatureOptional& x);

    void
    setSignature (::std::auto_ptr< SignatureType > p);

    // Constructors.
    //
    SidecarCompositionMapType (const IdType&,
                               const PropertiesType&,
                               const SidecarAssetListType&);

    SidecarCompositionMapType (const IdType&,
                               ::std::auto_ptr< PropertiesType >,
                               ::std::auto_ptr< SidecarAssetListType >);

    SidecarCompositionMapType (const ::xercesc::DOMElement& e,
                               ::xml_schema::Flags f = 0,
                               ::xml_schema::Container* c = 0);

    SidecarCompositionMapType (const SidecarCompositionMapType& x,
                               ::xml_schema::Flags f = 0,
                               ::xml_schema::Container* c = 0);

    virtual SidecarCompositionMapType*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    SidecarCompositionMapType&
    operator= (const SidecarCompositionMapType& x);

    virtual 
    ~SidecarCompositionMapType ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    ::xsd::cxx::tree::one< IdType > Id_;
    ::xsd::cxx::tree::one< PropertiesType > Properties_;
    ::xsd::cxx::tree::one< SidecarAssetListType > SidecarAssetList_;
    SignerOptional Signer_;
    SignatureOptional Signature_;
  };

  class SidecarAssetType: public ::xml_schema::Type
  {
    public:
    // Id
    //
    typedef ::dcml::UUIDType IdType;
    typedef ::xsd::cxx::tree::traits< IdType, char > IdTraits;

    const IdType&
    getId () const;

    IdType&
    getId ();

    void
    setId (const IdType& x);

    void
    setId (::std::auto_ptr< IdType > p);

    // AssociatedCPLList
    //
    typedef ::scm::SidecarAssetType_AssociatedCPLListType AssociatedCPLListType;
    typedef ::xsd::cxx::tree::traits< AssociatedCPLListType, char > AssociatedCPLListTraits;

    const AssociatedCPLListType&
    getAssociatedCPLList () const;

    AssociatedCPLListType&
    getAssociatedCPLList ();

    void
    setAssociatedCPLList (const AssociatedCPLListType& x);

    void
    setAssociatedCPLList (::std::auto_ptr< AssociatedCPLListType > p);

    // Constructors.
    //
    SidecarAssetType (const IdType&,
                      const AssociatedCPLListType&);

    SidecarAssetType (const IdType&,
                      ::std::auto_ptr< AssociatedCPLListType >);

    SidecarAssetType (const ::xercesc::DOMElement& e,
                      ::xml_schema::Flags f = 0,
                      ::xml_schema::Container* c = 0);

    SidecarAssetType (const SidecarAssetType& x,
                      ::xml_schema::Flags f = 0,
                      ::xml_schema::Container* c = 0);

    virtual SidecarAssetType*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    SidecarAssetType&
    operator= (const SidecarAssetType& x);

    virtual 
    ~SidecarAssetType ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    ::xsd::cxx::tree::one< IdType > Id_;
    ::xsd::cxx::tree::one< AssociatedCPLListType > AssociatedCPLList_;
  };

  class SidecarCompositionMapType_PropertiesType: public ::xml_schema::Type
  {
    public:
    // Annotation
    //
    typedef ::dcml::UserTextType AnnotationType;
    typedef ::xsd::cxx::tree::optional< AnnotationType > AnnotationOptional;
    typedef ::xsd::cxx::tree::traits< AnnotationType, char > AnnotationTraits;

    const AnnotationOptional&
    getAnnotation () const;

    AnnotationOptional&
    getAnnotation ();

    void
    setAnnotation (const AnnotationType& x);

    void
    setAnnotation (const AnnotationOptional& x);

    void
    setAnnotation (::std::auto_ptr< AnnotationType > p);

    // IssueDate
    //
    typedef ::xml_schema::DateTime IssueDateType;
    typedef ::xsd::cxx::tree::traits< IssueDateType, char > IssueDateTraits;

    const IssueDateType&
    getIssueDate () const;

    IssueDateType&
    getIssueDate ();

    void
    setIssueDate (const IssueDateType& x);

    void
    setIssueDate (::std::auto_ptr< IssueDateType > p);

    // Issuer
    //
    typedef ::dcml::UserTextType IssuerType;
    typedef ::xsd::cxx::tree::optional< IssuerType > IssuerOptional;
    typedef ::xsd::cxx::tree::traits< IssuerType, char > IssuerTraits;

    const IssuerOptional&
    getIssuer () const;

    IssuerOptional&
    getIssuer ();

    void
    setIssuer (const IssuerType& x);

    void
    setIssuer (const IssuerOptional& x);

    void
    setIssuer (::std::auto_ptr< IssuerType > p);

    // any
    //
    typedef ::xsd::cxx::tree::element_sequence AnySequence;
    typedef AnySequence::iterator AnyIterator;
    typedef AnySequence::const_iterator AnyConstIterator;

    const AnySequence&
    getAny () const;

    AnySequence&
    getAny ();

    void
    setAny (const AnySequence& s);

    // DOMDocument for wildcard content.
    //
    const ::xercesc::DOMDocument&
    getDomDocument () const;

    ::xercesc::DOMDocument&
    getDomDocument ();

    // Constructors.
    //
    SidecarCompositionMapType_PropertiesType (const IssueDateType&);

    SidecarCompositionMapType_PropertiesType (const ::xercesc::DOMElement& e,
                                              ::xml_schema::Flags f = 0,
                                              ::xml_schema::Container* c = 0);

    SidecarCompositionMapType_PropertiesType (const SidecarCompositionMapType_PropertiesType& x,
                                              ::xml_schema::Flags f = 0,
                                              ::xml_schema::Container* c = 0);

    virtual SidecarCompositionMapType_PropertiesType*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    SidecarCompositionMapType_PropertiesType&
    operator= (const SidecarCompositionMapType_PropertiesType& x);

    virtual 
    ~SidecarCompositionMapType_PropertiesType ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > dom_document_;

    AnnotationOptional Annotation_;
    ::xsd::cxx::tree::one< IssueDateType > IssueDate_;
    IssuerOptional Issuer_;
    AnySequence any_;
  };

  class SidecarCompositionMapType_SidecarAssetListType: public ::xml_schema::Type
  {
    public:
    // SidecarAsset
    //
    typedef ::scm::SidecarAssetType SidecarAssetType;
    typedef ::xsd::cxx::tree::sequence< SidecarAssetType > SidecarAssetSequence;
    typedef SidecarAssetSequence::iterator SidecarAssetIterator;
    typedef SidecarAssetSequence::const_iterator SidecarAssetConstIterator;
    typedef ::xsd::cxx::tree::traits< SidecarAssetType, char > SidecarAssetTraits;

    const SidecarAssetSequence&
    getSidecarAsset () const;

    SidecarAssetSequence&
    getSidecarAsset ();

    void
    setSidecarAsset (const SidecarAssetSequence& s);

    // Constructors.
    //
    SidecarCompositionMapType_SidecarAssetListType ();

    SidecarCompositionMapType_SidecarAssetListType (const ::xercesc::DOMElement& e,
                                                    ::xml_schema::Flags f = 0,
                                                    ::xml_schema::Container* c = 0);

    SidecarCompositionMapType_SidecarAssetListType (const SidecarCompositionMapType_SidecarAssetListType& x,
                                                    ::xml_schema::Flags f = 0,
                                                    ::xml_schema::Container* c = 0);

    virtual SidecarCompositionMapType_SidecarAssetListType*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    SidecarCompositionMapType_SidecarAssetListType&
    operator= (const SidecarCompositionMapType_SidecarAssetListType& x);

    virtual 
    ~SidecarCompositionMapType_SidecarAssetListType ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    SidecarAssetSequence SidecarAsset_;
  };

  class SidecarAssetType_AssociatedCPLListType: public ::xml_schema::Type
  {
    public:
    // CPLId
    //
    typedef ::dcml::UUIDType CPLIdType;
    typedef ::xsd::cxx::tree::sequence< CPLIdType > CPLIdSequence;
    typedef CPLIdSequence::iterator CPLIdIterator;
    typedef CPLIdSequence::const_iterator CPLIdConstIterator;
    typedef ::xsd::cxx::tree::traits< CPLIdType, char > CPLIdTraits;

    const CPLIdSequence&
    getCPLId () const;

    CPLIdSequence&
    getCPLId ();

    void
    setCPLId (const CPLIdSequence& s);

    // Constructors.
    //
    SidecarAssetType_AssociatedCPLListType ();

    SidecarAssetType_AssociatedCPLListType (const ::xercesc::DOMElement& e,
                                            ::xml_schema::Flags f = 0,
                                            ::xml_schema::Container* c = 0);

    SidecarAssetType_AssociatedCPLListType (const SidecarAssetType_AssociatedCPLListType& x,
                                            ::xml_schema::Flags f = 0,
                                            ::xml_schema::Container* c = 0);

    virtual SidecarAssetType_AssociatedCPLListType*
    _clone (::xml_schema::Flags f = 0,
            ::xml_schema::Container* c = 0) const;

    SidecarAssetType_AssociatedCPLListType&
    operator= (const SidecarAssetType_AssociatedCPLListType& x);

    virtual 
    ~SidecarAssetType_AssociatedCPLListType ();

    // Implementation.
    //
    protected:
    void
    parse (::xsd::cxx::xml::dom::parser< char >&,
           ::xml_schema::Flags);

    protected:
    CPLIdSequence CPLId_;
  };
}

#include <iosfwd>

namespace scm
{
  ::std::ostream&
  operator<< (::std::ostream&, const SidecarCompositionMapType&);

  ::std::ostream&
  operator<< (::std::ostream&, const SidecarAssetType&);

  ::std::ostream&
  operator<< (::std::ostream&, const SidecarCompositionMapType_PropertiesType&);

  ::std::ostream&
  operator<< (::std::ostream&, const SidecarCompositionMapType_SidecarAssetListType&);

  ::std::ostream&
  operator<< (::std::ostream&, const SidecarAssetType_AssociatedCPLListType&);
}

#include <iosfwd>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

namespace scm
{
  // Parse a URI or a local file.
  //

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (const ::std::string& uri,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (const ::std::string& uri,
                              ::xml_schema::ErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (const ::std::string& uri,
                              ::xercesc::DOMErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  // Parse std::istream.
  //

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              ::xml_schema::ErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              ::xercesc::DOMErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              const ::std::string& id,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              const ::std::string& id,
                              ::xml_schema::ErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::std::istream& is,
                              const ::std::string& id,
                              ::xercesc::DOMErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  // Parse xercesc::InputSource.
  //

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::xercesc::InputSource& is,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::xercesc::InputSource& is,
                              ::xml_schema::ErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::xercesc::InputSource& is,
                              ::xercesc::DOMErrorHandler& eh,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  // Parse xercesc::DOMDocument.
  //

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (const ::xercesc::DOMDocument& d,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());

  ::std::auto_ptr< ::scm::SidecarCompositionMapType >
  parseSidecarCompositionMap (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
                              ::xml_schema::Flags f = 0,
                              const ::xml_schema::Properties& p = ::xml_schema::Properties ());
}

#include <iosfwd>

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

namespace scm
{
  // Serialize to std::ostream.
  //

  void
  serializeSidecarCompositionMap (::std::ostream& os,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  void
  serializeSidecarCompositionMap (::std::ostream& os,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  ::xml_schema::ErrorHandler& eh,
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  void
  serializeSidecarCompositionMap (::std::ostream& os,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  ::xercesc::DOMErrorHandler& eh,
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  // Serialize to xercesc::XMLFormatTarget.
  //

  void
  serializeSidecarCompositionMap (::xercesc::XMLFormatTarget& ft,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  void
  serializeSidecarCompositionMap (::xercesc::XMLFormatTarget& ft,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  ::xml_schema::ErrorHandler& eh,
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  void
  serializeSidecarCompositionMap (::xercesc::XMLFormatTarget& ft,
                                  const ::scm::SidecarCompositionMapType& x, 
                                  ::xercesc::DOMErrorHandler& eh,
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  const ::std::string& e = "UTF-8",
                                  ::xml_schema::Flags f = 0);

  // Serialize to an existing xercesc::DOMDocument.
  //

  void
  serializeSidecarCompositionMap (::xercesc::DOMDocument& d,
                                  const ::scm::SidecarCompositionMapType& x,
                                  ::xml_schema::Flags f = 0);

  // Serialize to a new xercesc::DOMDocument.
  //

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >
  serializeSidecarCompositionMap (const ::scm::SidecarCompositionMapType& x, 
                                  const ::xml_schema::NamespaceInfomap& m = ::xml_schema::NamespaceInfomap (),
                                  ::xml_schema::Flags f = 0);

  void
  operator<< (::xercesc::DOMElement&, const SidecarCompositionMapType&);

  void
  operator<< (::xercesc::DOMElement&, const SidecarAssetType&);

  void
  operator<< (::xercesc::DOMElement&, const SidecarCompositionMapType_PropertiesType&);

  void
  operator<< (::xercesc::DOMElement&, const SidecarCompositionMapType_SidecarAssetListType&);

  void
  operator<< (::xercesc::DOMElement&, const SidecarAssetType_AssociatedCPLListType&);
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

#endif // SMPTE_2067_9A_2018_SIDECAR_H
