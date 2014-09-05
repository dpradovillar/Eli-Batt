<%@ page import="cl.renetapia.embeddedcontrol.ares.util.RequestHelper" %>
<%@ page import="cl.renetapia.embeddedcontrol.ares.util.BreadCrumb" %>
<%@ page contentType="text/html; charset=utf-8" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<div id="sidebar">

    <% int m = RequestHelper.getInt(request, "module"); %>
    <% int s = RequestHelper.getInt(request, "submodule"); %>

    <ul class="sideNav">
        <li><a href="view_all.html" <%= RequestHelper.active(s, 1, "active")%>>View all</a></li>
        <li><a href="add_new.html" <%= RequestHelper.active(s, 2, "active")%>>Add new</a></li>
        <li><a href="roles.html" <%= RequestHelper.active(s, 3, "active")%>>Roles</a></li>
    </ul>
    <!-- // .sideNav -->
</div>
<!-- // #sidebar -->

<!-- h2 stays for breadcrumbs -->
<!-- h2 stays for breadcrumbs -->
<%
    if(s<0) s=0;
    BreadCrumb[]brs = new BreadCrumb[]{
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Users", "index.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Users", "index.html").addSubmodule("View all", "view_all.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Users", "index.html").addSubmodule("Add new", "add_new.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Users", "index.html").addSubmodule("Roles", "roles.html", true),
    };
%>
<h2><%=  brs[s].toString() %></h2>

<div id="main">
    <%
    switch (s) {
        case 1: %><jsp:include page="user/view_all.jsp"/><% break;
        case 2: %><jsp:include page="user/add_new.jsp"/><% break;
        case 3: %><jsp:include page="user/roles.jsp"/><% break;
        default:  %><jsp:include page="user/main.jsp"/><% break;
    }%>
</div>
<!-- // #main -->