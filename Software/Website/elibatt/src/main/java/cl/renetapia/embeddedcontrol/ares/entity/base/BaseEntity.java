package cl.renetapia.embeddedcontrol.ares.entity.base;

import cl.renetapia.embeddedcontrol.ares.util.OmitInComparison;
import cl.renetapia.embeddedcontrol.ares.util.OmitInHashcode;
import cl.renetapia.embeddedcontrol.ares.util.OmitInToString;
import org.apache.commons.lang.builder.EqualsBuilder;
import org.apache.commons.lang.builder.HashCodeBuilder;
import org.apache.commons.lang.builder.ReflectionToStringBuilder;
import org.apache.commons.lang.builder.ToStringStyle;

import javax.persistence.MappedSuperclass;
import java.io.Serializable;
import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.util.LinkedList;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 27-04-13
 * Time: 09:45 PM
 * To change this template use File | Settings | File Templates.
 */

@MappedSuperclass
public abstract class BaseEntity implements Serializable {

    /**
     * <p>Recupera la lista de atributos que estan anotadas con la anotacion indicada.</p>
     * <p/> Por ejemplo:<p/>
     * <pre>class Foo {
     *   &#64;X String a;
     *   Object b;
     *   &#64;X int c;
     * }
     * <p/>
     * listFieldsWithAnnotation(Foo.class, X.class) retorna {"a","c"}
     * </pre>
     *
     * @param aClass     Clase que será escaneada reflexivamente.
     * @param annotation Anotación que será buscada en los atributos de la clase.
     * @return Lista con los nombres de los atributos que tienen la anotación presente.
     */
    public static List<String> listFieldsWithAnnotation(Class<?> aClass, Class<? extends Annotation> annotation) {
        List<String> result = new LinkedList<String>();
        for (Field field : aClass.getDeclaredFields()) {
            if (field.isAnnotationPresent(annotation)) {
                result.add(field.getName());
            }
        }
        return result;
    }

    @Override
    public int hashCode() {
        int val;
        synchronized (this) {
            List<String> omitedFields = listFieldsWithAnnotation(getClass(), OmitInHashcode.class);
            val = HashCodeBuilder.reflectionHashCode(this, omitedFields);
        }
        return val;
    }

    @Override
    public boolean equals(Object yours) {
        if (yours == null) {
            return false;
        }
        // bloquear ambos objetos, para acceder a sus campos via reflexion
        synchronized (this) {
            Class<?> thisClass = getClass();
            Class<?> yourClass = yours.getClass();
            if (thisClass == yourClass) {
                List<String> omitedFields = listFieldsWithAnnotation(thisClass, OmitInComparison.class);
                return EqualsBuilder.reflectionEquals(this, yours, omitedFields);
            }

        }
        return false;
    }

    @Override
    public String toString() {
        String str;
        synchronized (this) {
            str = (new ReflectionToStringBuilder(this, ToStringStyle.SHORT_PREFIX_STYLE) {
                protected boolean accept(Field f) {
                    if (super.accept(f)) {
                        // si la anotacion esta presente, excluimos el atributo del toString
                        if (!f.isAnnotationPresent(OmitInToString.class)) {
                            return true;
                        }
                    }
                    return false;
                }
            }).toString();
        }
        return str;
    }

    public String toJSON(){
        return "{}";
    }

}
