package cl.renetapia.embeddedcontrol.ares.business.service;

import cl.renetapia.embeddedcontrol.ares.entity.UserEntity;

import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: rene
 * Date: 01-11-13
 * Time: 05:02 PM
 * To change this template use File | Settings | File Templates.
 */
public interface IUserService {

    public UserEntity findByUsername(String username);

    public List<UserEntity> findAll();
}
