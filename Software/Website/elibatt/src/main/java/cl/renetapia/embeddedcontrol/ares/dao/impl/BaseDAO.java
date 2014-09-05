package cl.renetapia.embeddedcontrol.ares.dao.impl;

import cl.renetapia.embeddedcontrol.ares.dao.IBaseDAO;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.orm.hibernate3.support.HibernateDaoSupport;

import java.io.Serializable;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 31-03-13
 * Time: 03:49 PM
 * To change this template use File | Settings | File Templates.
 */
@SuppressWarnings("unchecked")
public class BaseDAO<T, S extends Serializable> extends HibernateDaoSupport implements IBaseDAO<T, S> {

    private Class<T> persistentClass;

    public BaseDAO(Class<T> persistentClass) {
        this.persistentClass = persistentClass;
    }

    @Autowired
    public void setFactory(SessionFactory sessionFactory) {
        super.setSessionFactory(sessionFactory);
    }

    protected Session session() {
        return getSessionFactory().getCurrentSession();
    }

    @Override
    public void flush() {
        session().flush();
    }

    @Override
    public T findFirst() {
        return findOneByStatement("from " + persistentClass.getSimpleName());
    }

    @Override
    public List<T> findAll() {
        return (List<T>) (session().createQuery("from " + persistentClass.getSimpleName()).list());
    }

    @Override
    public T findById(S pk) {
        return (T) session().get(persistentClass, pk);
    }

    @Override
    public void save(T object) {
        session().save(object);
    }

    @Override
    public void update(T object) {
        session().merge(object);
    }

    @Override
    public void delete(T object) {
        session().delete(object);
    }

    private List<T> genericFindByStatement(String statement, boolean uniqueResult, Object... paramPairs) {
        Query q = session().createQuery(statement);
        // establecer los parametros, 2 a 2 desde la lista variable de argumentos
        for (int i = 0; i < paramPairs.length; i += 2) {
            q.setParameter(String.valueOf(paramPairs[i]), paramPairs[i + 1]);
        }
        if (uniqueResult) {
            q.setMaxResults(1);
        }
        List<T> theResults = q.list();
        if (uniqueResult && theResults.size() == 0) {
            theResults.add(null);
        }
        return theResults;
    }

    @Override
    public List<T> findByStatement(String statement, Object... paramPairs) {
        return genericFindByStatement(statement, false, paramPairs);
    }

    @Override
    public T findOneByStatement(String statement, Object... paramPairs) {
        return genericFindByStatement(statement, true, paramPairs).get(0);
    }

    @Override
    public void deleteAll() {
        Query query = session().createQuery("delete from " + persistentClass.getSimpleName());
        query.executeUpdate();
    }

}
