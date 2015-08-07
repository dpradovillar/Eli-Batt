<%@ page import="cl.renetapia.embeddedcontrol.ares.util.RequestHelper" %>
<%@ page import="cl.renetapia.embeddedcontrol.ares.util.BreadCrumb" %>
<%@ page contentType="text/html; charset=utf-8" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/functions" prefix="fn" %>

<div id="sidebar">

    <% int m = RequestHelper.getInt(request, "module"); %>
    <% int s = RequestHelper.getInt(request, "submodule"); %>

    <ul class="sideNav">
        <li><a href="raw_data.html" <%= RequestHelper.active(s, 1, "active")%>>Raw data</a></li>
        <li><a href="graphs.html" <%= RequestHelper.active(s, 2, "active")%>>Graphs</a></li>
        <li><a href="reports.html" <%= RequestHelper.active(s, 3, "active")%>>Reports</a></li>
    </ul>
    <!-- // .sideNav -->
</div>
<!-- // #sidebar -->

<!-- h2 stays for breadcrumbs -->
<!-- h2 stays for breadcrumbs -->
<%
    if(s<0) s=0;
    BreadCrumb[]brs = new BreadCrumb[]{
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Data", "index.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Data", "index.html").addSubmodule("Raw data", "raw_data.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Data", "index.html").addSubmodule("Graphs", "graphs.html", true),
            new BreadCrumb().addRoot("Elibatt", "../main/index.html").addModule("Data", "index.html").addSubmodule("Reports", "reports.html", true),
    };
%>
<h2><%=  brs[s].toString() %></h2>

<div id="main">
    <%
    switch (s) {
        case 1: %><jsp:include page="data/raw_data.jsp"/><% break;
        case 2: %><jsp:include page="data/graphs.jsp"/><% break;
        case 3: %><jsp:include page="data/reports.jsp"/><% break;
        default: %><jsp:include page="data/main.jsp"/><% break;
    } %>
</div>
<!-- // #main -->