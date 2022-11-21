import org.apache.commons.configuration2.interpol.ConfigurationInterpolator;
import org.apache.commons.configuration2.interpol.InterpolatorSpecification;

public class CVE_2022_33980 {
    private static String onInput(String input) {
        InterpolatorSpecification spec = new InterpolatorSpecification.Builder()
                .withPrefixLookups(ConfigurationInterpolator.getDefaultPrefixLookups())
                .withDefaultLookups(ConfigurationInterpolator.getDefaultPrefixLookups().values())
                .create();

        ConfigurationInterpolator interpolator = ConfigurationInterpolator.fromSpecification(spec);
        return interpolator.interpolate(input).toString();
    }

    public static void main(String[] args) {
        while (true) {
            System.out.print("Input: ");
            String input = System.console().readLine();
            String output = onInput(input);
            System.out.println("Output: " + output);
        }
    }
}
