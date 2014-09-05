package cl.renetapia.embeddedcontrol.ares.business.application;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.transaction.annotation.Transactional;

import javax.servlet.ServletContext;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 27-06-13
 * Time: 01:09 AM
 * To change this template use File | Settings | File Templates.
 */
public class ApplicationListenerBean implements ApplicationListener<ContextRefreshedEvent> {
    @Autowired ServletContext context;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public void onApplicationEvent(ContextRefreshedEvent event) {
        String working = context.getInitParameter("working-location");
        String downloads = context.getInitParameter("downloads-location");
        ApplicationParameters.getInstance().setParameters(working, downloads);
    }

}
