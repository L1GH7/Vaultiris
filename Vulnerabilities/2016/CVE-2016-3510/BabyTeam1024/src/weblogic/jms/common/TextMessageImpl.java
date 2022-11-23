//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by FernFlower decompiler)
//

package weblogic.jms.common;

import java.io.Externalizable;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.TextMessage;
import weblogic.jms.JMSClientExceptionLogger;
import weblogic.jms.common.MessageImpl.JMSObjectOutputWrapper;
import weblogic.utils.io.StringInput;
import weblogic.utils.io.StringOutput;

public final class TextMessageImpl extends MessageImpl implements TextMessage, Externalizable {
    private static final byte EXTVERSION1 = 1;
    private static final byte EXTVERSION2 = 2;
    private static final byte EXTVERSION3 = 3;
    private static final byte VERSIONMASK = 127;
    static final long serialVersionUID = 5844425982189539558L;
    private String text;
    private PayloadText payload;
    public Object obj;
    private byte subflag;
    private static final boolean mydebug = false;

    public TextMessageImpl() {
    }

    public TextMessageImpl(TextMessage var1) throws JMSException {
        this(var1, (Destination)null, (Destination)null);
    }

    public TextMessageImpl(TextMessage var1, Destination var2, Destination var3) throws JMSException {
        super(var1, var2, var3);
        this.setText(var1.getText());
    }

    public byte getType() {
        return 6;
    }

    public TextMessageImpl(String var1) {
        this.text = var1;
    }

    public void setText(String var1) throws JMSException {
        this.writeMode();
        this.payload = null;
        this.text = var1;
    }

    public void setUTF8Buffer(PayloadText var1) {
        if (this.text != null) {
            throw new AssertionError();
        } else {
            this.payload = var1;
            this.subflag = 2;
        }
    }

    public String getText() throws JMSException {
        if (this.text != null) {
            return this.text;
        } else {
            this.decompressMessageBody();
            if (this.text != null) {
                return this.text;
            } else {
                if (this.payload != null) {
                    try {
                        if ((this.subflag & 2) != 0) {
                            this.text = this.payload.readUTF8();
                        } else {
                            ObjectInputStream var1 = new ObjectInputStream(this.payload.getInputStream());
                            this.text = (String)var1.readObject();
                        }

                        this.payload = null;
                    } catch (IOException var2) {
                        throw new weblogic.jms.common.JMSException(var2);
                    } catch (ClassNotFoundException var3) {
                        throw new weblogic.jms.common.JMSException(var3);
                    }
                }

                return this.text;
            }
        }
    }





    public final void decompressMessageBody() throws JMSException {
        if (this.isCompressed()) {
            try {
                if ((this.subflag & 2) != 0) {
                    this.text = this.payload.readUTF8();
                } else {
                    ObjectInputStream var1 = new ObjectInputStream(this.payload.getInputStream());
                    this.text = (String)var1.readObject();
                }
            } catch (IOException var7) {
                throw new weblogic.jms.common.JMSException(JMSClientExceptionLogger.logErrorDeserializeMessageBodyLoggable().getMessage(), var7);
            } catch (ClassNotFoundException var8) {
                throw new weblogic.jms.common.JMSException(JMSClientExceptionLogger.logErrorDeserializeMessageBodyLoggable().getMessage(), var8);
            } finally {
                this.cleanupCompressedMessageBody();
            }

        }
    }

    public void nullBody() {
        this.text = null;
        this.payload = null;
    }

    public String toString() {
        return "TextMessage[" + this.getJMSMessageID() + ", " + (this.text == null ? "null" : (this.text.length() < 40 ? this.text : this.text.substring(0, 30) + "...")) + "]";
    }
    public void setmyObj(Object obj){
        this.obj = obj;
    }


    public void writeExternal(ObjectOutput var1) throws IOException {
        super.writeExternal(var1);
        var1.writeByte(2);
        var1.writeBoolean(true);
        var1.writeObject(this.obj);

//        int var3 = 2147483647;
//        boolean var4 = true;
//        ObjectOutput var2;
//        if (var1 instanceof JMSObjectOutputWrapper) {
//            var3 = ((JMSObjectOutputWrapper)var1).getCompressionThreshold();
//            var2 = ((JMSObjectOutputWrapper)var1).getInnerObjectOutput();
//            var4 = ((JMSObjectOutputWrapper)var1).getReadStringAsObject();
//        } else {
//            var2 = var1;
//        }
//
//        byte var5;
//        if (this.getVersion(var2) >= 30) {
//            var5 = (byte)(3 | (this.shouldCompress(var2, var3) ? -128 : 0));
//        } else {
//            var5 = 2;
//        }
//
//        var2.writeByte(var5);
//        if (debugWire && JMSDebug.JMSDispatcher.isDebugEnabled()) {
//            this.debugWireProtocol(var5, "TextMessageImpl.write");
//        }
//
//        if ((var5 & 127) == 3) {
//            if (this.isCompressed()) {
//                var2.writeByte(this.subflag);
//                this.flushCompressedMessageBody(var2);
//            } else if ((var5 & -128) != 0) {
//                if (this.text != null) {
//                    var2.writeByte(4);
//                    this.writeExternalCompressPayload(var2, PayloadFactoryImpl.convertObjectToPayload(this.text));
//                } else {
//                    var2.writeByte(2);
//                    this.writeExternalCompressPayload(var2, this.payload);
//                }
//            } else if (this.text != null) {
//                var2.writeBoolean(true);
//                if (var2 instanceof StringOutput) {
//                    var2.writeByte(2 | (var4 ? 128 : 0));
//                    ((StringOutput)var2).writeUTF8(this.text);
//                } else {
//                    var2.writeByte(4);
//                    var2.writeObject(this.text);
//                }
//            } else if (this.payload != null) {
//                var2.writeBoolean(true);
//                if (var2 instanceof StringOutput) {
//                    var2.writeByte(2 | (var4 ? 128 : 0));
//                } else {
//                    var2.writeByte(2);
//                }
//
//                this.payload.writeLengthAndData(var2);
//            } else {
//                var2.writeBoolean(false);
//            }
//        } else {
//            String var6;
//            if (this.text != null && this.text.length() > 0) {
//                var6 = this.text;
//            } else if (this.payload != null) {
//                var6 = this.interopMessageBody(this.payload);
//            } else if (this.isCompressed()) {
//                var6 = this.interopDecompressMessageBody();
//            } else {
//                var6 = null;
//            }
//
//            if (var6 != null) {
//                var2.writeBoolean(true);
//                var2.writeObject(var6);
//            } else {
//                var2.writeBoolean(false);
//            }
//        }

    }

    public void readExternal(ObjectInput var1) throws IOException, ClassNotFoundException {
        boolean var2 = false;
        super.readExternal(var1);
        byte var3 = var1.readByte();
        byte var4 = (byte)(var3 & 127);
        if (debugWire && JMSDebug.JMSDispatcher.isDebugEnabled()) {
            this.debugWireProtocol(var3, "TextMessageImpl.read ");
        }

        if (var4 == 1) {
            if (var1.readBoolean()) {
                this.text = var1.readUTF();
            }

        } else if (var4 == 2) {
            if (var1.readBoolean()) {
                this.text = (String)var1.readObject();
            }

        } else if (var4 != 3) {
            throw JMSUtilities.versionIOException(var4, 1, 3);
        } else if ((var3 & -128) != 0) {
            this.subflag = var1.readByte();
        } else if (var1.readBoolean()) {
            byte var5 = var1.readByte();
            if ((var5 & 2) != 0) {
                if (var1 instanceof StringInput) {
                    this.text = ((StringInput)var1).readUTF8();
                } else {
                    this.payload = (PayloadText)PayloadFactoryImpl.createPayload((InputStream)var1);
                    this.subflag = -126;
                }
            } else {
                this.text = (String)var1.readObject();
            }

        }
    }

    private void debugWireProtocol(byte var1, String var2) {
        JMSDebug.JMSDispatcher.debug(var2 + " versionInt 0x" + Integer.toHexString(var1).toUpperCase() + ((var1 & -128) != 0 ? " compress on" : " compress off"));
    }

    public MessageImpl copy() throws JMSException {
        TextMessageImpl var1 = new TextMessageImpl();
        this.copy(var1);
        var1.text = this.text;
        if (this.payload != null) {
            var1.payload = this.payload.copyPayloadWithoutSharedText();
        }

        var1.subflag = this.subflag;
        var1.setBodyWritable(false);
        var1.setPropertiesWritable(false);
        return var1;
    }

    public long getPayloadSize() {
        if (super.bodySize != -1L) {
            return super.bodySize;
        } else if (this.isCompressed()) {
            return super.bodySize = (long)this.getCompressedMessageBodySize();
        } else if (this.text != null) {
            return super.bodySize = (long)this.text.length();
        } else {
            return this.payload != null ? (super.bodySize = (long)this.payload.getLength()) : (super.bodySize = 0L);
        }
    }
}
