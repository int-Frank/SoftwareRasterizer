#ifndef XMLVALIDATOR_H
#define XMLVALIDATOR_H

#include <string>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/validators/common/Grammar.hpp>

namespace impl
{
    class ParserErrorHandler : public xercesc::ErrorHandler
    {
    private:
        void reportParseException(const xercesc::SAXParseException& ex);

    public:
        void warning(const xercesc::SAXParseException& ex)
        {
            reportParseException(ex);
        }

        void error(const xercesc::SAXParseException& ex)
        {
            reportParseException(ex);
        }

        void fatalError(const xercesc::SAXParseException& ex)
        {
            reportParseException(ex);
        }

        void resetErrors()
        {
        }
    };
}


class XMLValidator
{
public:

    XMLValidator() : m_isReady(false) {}

    virtual bool ValidateXML(const std::string& file) { return ValidateXMLFromSchema(file); }
    bool SetSchema(const std::string& file);
    bool IsReady() const { return m_isReady; }

protected:

    bool ValidateXMLFromSchema(const std::string& file);

private:

    impl::ParserErrorHandler m_parserErrorHandler;
    xercesc::XercesDOMParser m_domParser;
    bool m_isReady;

};

#endif