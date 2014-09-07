package cl.renetapia.embeddedcontrol.ares.util;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 01-11-13
 * Time: 06:50 PM
 * To change this template use File | Settings | File Templates.
 */
public class BreadCrumb {

    private String buffer = "";

    public BreadCrumb() {
    }

    // ----------------------

    public BreadCrumb addRoot(String text, String url, boolean active) {
        buffer = link(text, url, active);
        return this;
    }

    public BreadCrumb addRoot(String text, String url) {
        return addRoot(text, url, false);
    }

    // ----------------------

    public BreadCrumb addModule(String text, String url, boolean active) {
        buffer += " &raquo; ";
        buffer += link(text, url, active);
        return this;
    }

    public BreadCrumb addModule(String text, String url) {
        return addModule(text, url, false);
    }

    // ----------------------

    public BreadCrumb addSubmodule(String text, String url, boolean active) {
        buffer += " &raquo; ";
        buffer += link(text, url, active);
        return this;
    }

    public BreadCrumb addSubmodule(String text, String url) {
        return addSubmodule(text, url, false);
    }

    // ----------------------

    public static String link(String text, String path, boolean active) {
        if (path.startsWith("www.")) {
            path = "http://" + path;
        }
        return String.format("<a href=\"%s\" %s>%s</a>", path, (active ? "class=\"active\"" : ""), text);
    }

    @Override
    public String toString() {
        return buffer;
    }

}
