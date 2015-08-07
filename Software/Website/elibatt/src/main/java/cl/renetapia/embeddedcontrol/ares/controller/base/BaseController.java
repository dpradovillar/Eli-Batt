package cl.renetapia.embeddedcontrol.ares.controller.base;

import org.json.simple.JSONObject;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 01-11-13
 * Time: 05:38 PM
 * To change this template use File | Settings | File Templates.
 */
public class BaseController {

    private int module;

    public BaseController(int module) {
        this.module = module;
    }

    @RequestMapping(value = "/error.html", method = RequestMethod.GET)
    public ModelAndView error(
            HttpServletRequest request,
            HttpServletResponse response
    ) {
        return new ModelAndView("frame").addObject("module", -1);
    }

    protected ModelAndView ms(int submodule) {
        return new ModelAndView("frame")
                .addObject("module", module)
                .addObject("submodule", submodule)
                ;
    }

    protected ModelAndView ms() {
        return ms(-1);
    }


    protected String errorJSON(Exception e){
        e.printStackTrace();
        JSONObject obj = new JSONObject();
        obj.put("error", e.getMessage());
        return obj.toJSONString();
    }
}
