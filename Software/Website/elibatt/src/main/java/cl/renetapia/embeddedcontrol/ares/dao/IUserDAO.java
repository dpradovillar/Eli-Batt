package cl.renetapia.embeddedcontrol.ares.dao;

import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 26-04-13
 * Time: 11:55 PM
 * To change this template use File | Settings | File Templates.
 */
public interface IUserDAO extends IBaseDAO<UserEntity, Integer> {

    public UserEntity findByUsername(String username);
}
