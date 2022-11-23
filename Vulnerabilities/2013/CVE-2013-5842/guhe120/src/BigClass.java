import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.Serializable;


public class BigClass implements Serializable {
	
	private int i0_;
	private int i1_;
	private int i2_;
	private int i3_;
	private int i4_;
	private int i5_;
	private int i6_;
	private int i7_;
	private int i8_;
	private int i9_;
	
	

	
	private synchronized void writeObject(ObjectOutputStream stream)
	         throws IOException {
		try {Thread.sleep(1000000000);} catch(Exception e) {}
	}
}
