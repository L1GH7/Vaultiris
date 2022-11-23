package com.tw;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.StringReader;
import java.util.stream.Collectors;
import com.google.gson.Gson;

import com.tw.com.bean.TestBean;
import de.laures.cewolf.CewolfRenderer;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

@WebServlet(name="HelloWorld", urlPatterns={"/hello"},loadOnStartup=1)
public class TestServlet extends HttpServlet {

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        response.getWriter().append("You can request with POST method and body with XML and get hint with GET method to read pom.xml and web.xml.") ;
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        String body = request.getReader().lines().collect(Collectors.joining(System.lineSeparator()));

        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setIgnoringElementContentWhitespace(true);
        try {
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document document = db.parse(new InputSource(new StringReader(body)));
            response.getWriter().append("I got your XML and Parse OK.") ;
        } catch (ParserConfigurationException e) {
            response.getWriter().append(e.toString());
            throw new RuntimeException(e);
        } catch (SAXException e) {
            response.getWriter().append(e.toString());
            throw new RuntimeException(e);
        }


    }


}
