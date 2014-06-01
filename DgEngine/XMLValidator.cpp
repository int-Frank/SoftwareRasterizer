#include "XMLValidator.h"
#include <iostream>

void impl::ParserErrorHandler::reportParseException(const xercesc::SAXParseException& ex)
{
    char* msg = xercesc::XMLString::transcode(ex.getMessage());

    std::cerr << "Error at line " << ex.getLineNumber() << " column " <<
        ex.getColumnNumber() << ", " << std::string(msg) << std::endl;

    xercesc::XMLString::release(&msg);
}


bool XMLValidator::SetSchema(const std::string& file)
{
    if (m_domParser.loadGrammar(file.c_str(), xercesc::Grammar::SchemaGrammarType) == NULL)
    {
        std::cerr << "@XMLValidator::SetSchema() -> Could not load Schema: " << file << std::endl;
        m_isReady = false;
    }
    else
    {
        m_domParser.setErrorHandler(&m_parserErrorHandler);
        m_domParser.setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
        m_domParser.setDoNamespaces(true);
        m_domParser.setDoSchema(true);
        m_domParser.setValidationConstraintFatal(true);

        m_isReady = true;
    }

    return m_isReady;
}


bool XMLValidator::ValidateXMLFromSchema(const std::string& file)
{
    if (!m_isReady)
    {
        return false;
    }

    m_domParser.parse(file.c_str());
    if (m_domParser.getErrorCount() == 0)
    {
        return true;
    }

    std::cerr << "@XMLValidator::ValidateXML() -> XML file doesn't conform to the schema: " << file << std::endl;
    return false;
}