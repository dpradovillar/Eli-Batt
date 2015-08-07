package cl.renetapia.embeddedcontrol.ares.business.application;

/**
 * Created by rene on 27-08-2014.
 */
public class ApplicationParameters {

    private static ApplicationParameters ourInstance;

    public static ApplicationParameters getInstance() {
        if (ourInstance == null) {
            ourInstance = new ApplicationParameters();
        }
        return ourInstance;
    }

    private String workingLocation = "./working-location";
    private String downloadsLocation = "./downloads-location";

    private ApplicationParameters() {
    }

    public void setParameters(String workingLocation, String downloadsLocation) {
        this.workingLocation = workingLocation;
        this.downloadsLocation = downloadsLocation;
    }

    public String getWorkingLocation() {
        return workingLocation;
    }

    public String getDownloadsLocation() {
        return downloadsLocation;
    }

}
