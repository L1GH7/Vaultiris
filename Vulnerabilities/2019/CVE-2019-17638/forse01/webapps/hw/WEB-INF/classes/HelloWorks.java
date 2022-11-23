import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class HelloWorks extends HttpServlet {
private static final long serialVersionUID = 1L;

  public void doGet(HttpServletRequest request, HttpServletResponse response)
  throws IOException, ServletException
  {
    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.println("<head>");
    out.println("<title>HelloWorks</title>");
    out.println("</head>");
    out.println("<body>");
    out.println("<h1>HelloWorks (9.4.27)</h1>");
    out.println("</body>");
    out.println("</html>");
  }
}
