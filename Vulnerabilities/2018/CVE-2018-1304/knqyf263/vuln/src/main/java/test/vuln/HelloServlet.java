package test.vuln;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.annotation.HttpMethodConstraint;
import javax.servlet.annotation.HttpConstraint;
import javax.servlet.annotation.ServletSecurity;
import javax.servlet.annotation.WebServlet;

@WebServlet (name = "Root", urlPatterns = { "" })
@ServletSecurity(value=@HttpConstraint(rolesAllowed={"admin"}))
public class HelloServlet extends HttpServlet {

  public void doGet(HttpServletRequest request, HttpServletResponse response)
    throws IOException, ServletException{

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.println("<h1>CVE-2018-1304!!</h1>");
    out.println("</body>");
    out.println("</html>");
  }
}
