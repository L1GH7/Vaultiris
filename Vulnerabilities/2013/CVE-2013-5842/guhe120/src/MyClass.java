import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class MyClass implements Serializable {
	
		public static int cnt_ = 0;
		public MyClass p_;
		public int i1_ = 0x41414141;
		public int i2_ = 0x42424242;
		public int i3_ = 0x43434343;
		public int i4_ = 0x44444444;
		public int i5_ = 0x45454545;
	
		
		MyClass() 
		{
		}
		
		public void setPointer(MyClass p) {
			p_ = p;
		}
		
		
	
		private synchronized void writeObject(ObjectOutputStream stream)
		         throws IOException {
			
			if ( cnt_ ++ == 1 ) {
				this.i4_ = 0x7fffffff;
				this.i5_ = 0x7fffffff;
				return;
			}
			
			System.out.println("Write " + this.getClass());
			
			TestDeserialize.notifyMainThread();
			
			stream.defaultWriteObjectSlow();
			
			System.out.println("Finished writeObject.");
		}
	}