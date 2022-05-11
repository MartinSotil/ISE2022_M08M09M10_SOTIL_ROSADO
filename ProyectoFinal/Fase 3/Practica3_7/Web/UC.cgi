t <html><head><title>Unidad Control </title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t var formUpdate = new periodicObj("UC.cgx", 500);
t function plotRTCGraph() {
t  RTCVal = document.getElementById("RTC_value").value;
t }
t function PUC() {
t   updateMultiple(formUpdate,plotRTCGraph);
t   RTC_elTime = setTimeout(PUC, formUpdate.period);
t }
t </script></head>
t </script></head>
i pg_header.inc
t <br>
t <p><font size="4">desde esta pagina controlamos la ganancia y el nivel de overload del amplificador</font></p><br>
t <form action="UC.cgi" method="post" name="UC1">
t <input type="hidden" value="UC1" name="pg1">
t <font size="2">Ganancia Actual: &nbsp&nbsp&nbsp</font>
t <input type="text" readonly style="background-color: transparent; border: 0px"
c j size="10" id="Ganancia" value="%s"><br><br>
t <font size="2">Elegir ganancia del amplificador&nbsp&nbsp</font><select name="ctrl" onchange="submit();">
c m <option %s>--</option><option %s>1</option><option %s>5</option><option %s>10</option>
t <option %s>50</option><option %s>100</option></select><br><br>
t <font size="2"> Valor Overload actual:</font>
t <input type="text" readonly style="background-color: transparent; border: 0px"
c a size="10" id="Overload" value="%s"><br><br>
t </form>
t <script>  PUC() </script>
t <form action="UC.cgi" method="post" name="UC2">
t <input type="hidden" value="UC2" name="pg1">
t <font size="2">Introduzca nuevo Overload &nbsp&nbsp</font>
c f 1 <input type=text name=lcd2 size=20 maxlength=10 value="%s">
t <input type=submit name=set value="Send" id="sbm" ><br><br>
t <font size="2">Estado Overload: &nbsp&nbsp&nbsp</font>
t <input type="text" readonly style="background-color: transparent; border: 0px"
c l size="20" id="RTC_value" value="%s"><br><br>
t </form>
t <form action="UC.cgi" method="post" name="UC3">
t <input type="hidden" value="led" name="pg1">
t <font size="2">Activar Overload: &nbsp&nbsp&nbsp</font>
t <select name="ctrl" onchange="submit();">
c b <option %s>HABILITADO</option><option %s>DESHABILITADO</option></select><br><br>
t </form>
i pg_footer.inc
. End of script must be closed with period