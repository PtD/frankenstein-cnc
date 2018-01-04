<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.7.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="6" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-pulse">
<description>&lt;b&gt;Pulse Engineering, Inc.&lt;/b&gt;&lt;p&gt;
www.pulseeng.com
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="J1">
<description>&lt;b&gt;PULSEJACK (TM) 1x1 Tab-UP RJ45&lt;/b&gt;&lt;p&gt;
10/100 Base-TX RJ45 1x1 Tab-UP with LEDs 8-pin (J1 series) and 6-pin (JP series) integrated magnetics connector,&lt;br&gt;
designed to support applications, such as ADSL modems, LAN-on-Motherboard, and Hub and Switches.&lt;br&gt;
Source: www.pulseeng.com .. PulseJack-J402.pdf</description>
<wire x1="-8.405" y1="-17.05" x2="8.405" y2="-17.05" width="0.2032" layer="21"/>
<wire x1="8.405" y1="-17.05" x2="8.405" y2="8.15" width="0.2032" layer="51"/>
<wire x1="-8.405" y1="8.15" x2="-8.405" y2="-17.05" width="0.2032" layer="51"/>
<wire x1="-8.405" y1="8.15" x2="8.405" y2="8.15" width="0.2032" layer="21"/>
<wire x1="-8.3851" y1="-16.9799" x2="-8.8044" y2="-16.316" width="0.2032" layer="21"/>
<wire x1="-8.8044" y1="-16.316" x2="-10.2718" y2="-10.3067" width="0.2032" layer="21" curve="-37.105316"/>
<wire x1="-10.2718" y1="-10.3067" x2="-10.5862" y2="-9.2586" width="0.2032" layer="21" curve="43.052296"/>
<wire x1="-10.5862" y1="-9.2586" x2="-10.4814" y2="-9.1538" width="0.2032" layer="21" curve="-166.504203"/>
<wire x1="-10.4814" y1="-9.1538" x2="-10.3766" y2="-9.2935" width="0.2032" layer="21"/>
<wire x1="-10.3766" y1="-9.2935" x2="-10.132" y2="-9.9923" width="0.2032" layer="21" curve="-35.168494"/>
<wire x1="-10.132" y1="-9.9923" x2="-10.132" y2="-10.726" width="0.2032" layer="21" curve="-3.414373"/>
<wire x1="-10.132" y1="-10.726" x2="-8.455" y2="-16.5606" width="0.2032" layer="21" curve="35.485558"/>
<wire x1="8.3851" y1="-16.9799" x2="8.8044" y2="-16.316" width="0.2032" layer="21"/>
<wire x1="8.8044" y1="-16.316" x2="10.2718" y2="-10.3067" width="0.2032" layer="21" curve="37.105316"/>
<wire x1="10.2718" y1="-10.3067" x2="10.5862" y2="-9.2586" width="0.2032" layer="21" curve="-43.052296"/>
<wire x1="10.5862" y1="-9.2586" x2="10.4814" y2="-9.1538" width="0.2032" layer="21" curve="166.504203"/>
<wire x1="10.4814" y1="-9.1538" x2="10.3766" y2="-9.2935" width="0.2032" layer="21"/>
<wire x1="10.3766" y1="-9.2935" x2="10.132" y2="-9.9923" width="0.2032" layer="21" curve="35.168494"/>
<wire x1="10.132" y1="-9.9923" x2="10.132" y2="-10.726" width="0.2032" layer="21" curve="3.414373"/>
<wire x1="10.132" y1="-10.726" x2="8.455" y2="-16.5606" width="0.2032" layer="21" curve="-35.485558"/>
<wire x1="-8.405" y1="8.15" x2="-8.405" y2="-1.27" width="0.2032" layer="21"/>
<wire x1="-8.405" y1="-5.08" x2="-8.405" y2="-17.05" width="0.2032" layer="21"/>
<wire x1="8.405" y1="-17.05" x2="8.405" y2="-5.08" width="0.2032" layer="21"/>
<wire x1="8.405" y1="-1.27" x2="8.405" y2="8.15" width="0.2032" layer="21"/>
<wire x1="-6.8922" y1="-16.9332" x2="-6.5774" y2="-8.8515" width="0.2032" layer="21"/>
<wire x1="-6.5774" y1="-8.8515" x2="-6.2975" y2="-8.5716" width="0.2032" layer="21" curve="-85.555511"/>
<wire x1="-6.3675" y1="-16.9332" x2="-6.1575" y2="-9.8661" width="0.2032" layer="21"/>
<wire x1="-6.1575" y1="-9.8661" x2="-4.6531" y2="-9.8661" width="0.2032" layer="21" curve="-176.58855"/>
<wire x1="-4.6531" y1="-9.8661" x2="-4.4082" y2="-16.9682" width="0.2032" layer="21"/>
<wire x1="-3.8834" y1="-16.9332" x2="-4.1982" y2="-8.8515" width="0.2032" layer="21"/>
<wire x1="-4.1982" y1="-8.8515" x2="-4.4781" y2="-8.5716" width="0.2032" layer="21" curve="85.555511"/>
<wire x1="-6.2975" y1="-8.5716" x2="-4.4781" y2="-8.5716" width="0.2032" layer="21"/>
<wire x1="6.8922" y1="-16.9332" x2="6.5774" y2="-8.8515" width="0.2032" layer="21"/>
<wire x1="6.5774" y1="-8.8515" x2="6.2975" y2="-8.5716" width="0.2032" layer="21" curve="85.555511"/>
<wire x1="6.3675" y1="-16.9332" x2="6.1575" y2="-9.8661" width="0.2032" layer="21"/>
<wire x1="6.1575" y1="-9.8661" x2="4.6531" y2="-9.8661" width="0.2032" layer="21" curve="176.58855"/>
<wire x1="4.6531" y1="-9.8661" x2="4.4082" y2="-16.9682" width="0.2032" layer="21"/>
<wire x1="3.8834" y1="-16.9332" x2="4.1982" y2="-8.8515" width="0.2032" layer="21"/>
<wire x1="4.1982" y1="-8.8515" x2="4.4781" y2="-8.5716" width="0.2032" layer="21" curve="-85.555511"/>
<wire x1="6.2975" y1="-8.5716" x2="4.4781" y2="-8.5716" width="0.2032" layer="21"/>
<pad name="1" x="4.445" y="2.54" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="2" x="3.175" y="0" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="3" x="1.905" y="2.54" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="4" x="0.635" y="0" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="5" x="-0.635" y="2.54" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="6" x="-1.905" y="0" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="7" x="-3.175" y="2.54" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="8" x="-4.445" y="0" drill="0.9" diameter="1.9304" shape="square"/>
<pad name="M1" x="-7.875" y="-3.3" drill="1.6" diameter="2.1844" shape="square"/>
<pad name="M2" x="7.875" y="-3.3" drill="1.6" diameter="2.1844" shape="square"/>
<pad name="9" x="5.465" y="7.37" drill="1" diameter="1.4224" shape="square"/>
<pad name="12" x="-5.465" y="7.37" drill="1" diameter="1.4224" shape="square"/>
<pad name="10" x="2.925" y="7.37" drill="1" diameter="1.4224" shape="square"/>
<pad name="11" x="-2.925" y="7.37" drill="1" diameter="1.4224" shape="square"/>
<text x="-7.62" y="8.89" size="1.27" layer="25">&gt;NAME</text>
<text x="-6.35" y="4.445" size="1.27" layer="27">&gt;VALUE</text>
<hole x="5.715" y="-6.35" drill="3.3"/>
<hole x="-5.715" y="-6.35" drill="3.3"/>
</package>
</packages>
<symbols>
<symbol name="J006-LED">
<wire x1="21.59" y1="-36.83" x2="20.32" y2="-39.37" width="0.254" layer="94"/>
<wire x1="20.32" y1="-39.37" x2="19.05" y2="-36.83" width="0.254" layer="94"/>
<wire x1="21.59" y1="-39.37" x2="20.32" y2="-39.37" width="0.254" layer="94"/>
<wire x1="20.32" y1="-39.37" x2="19.05" y2="-39.37" width="0.254" layer="94"/>
<wire x1="21.59" y1="-36.83" x2="20.32" y2="-36.83" width="0.254" layer="94"/>
<wire x1="20.32" y1="-36.83" x2="19.05" y2="-36.83" width="0.254" layer="94"/>
<wire x1="20.32" y1="-36.83" x2="20.32" y2="-39.37" width="0.1524" layer="94"/>
<wire x1="18.288" y1="-37.592" x2="16.891" y2="-38.989" width="0.1524" layer="94"/>
<wire x1="18.415" y1="-38.735" x2="17.018" y2="-40.132" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-40.64" x2="20.32" y2="-39.37" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="2.54" x2="-3.175" y2="2.54" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="10.16" x2="-3.175" y2="10.16" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="17.78" x2="-3.175" y2="17.78" width="0.1524" layer="94"/>
<wire x1="-3.175" y1="10.16" x2="-3.175" y2="12.7" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="12.7" x2="-3.175" y2="15.24" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="15.24" x2="-3.175" y2="17.78" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="7.62" x2="-3.175" y2="10.16" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="5.08" x2="-3.175" y2="7.62" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="2.54" x2="-3.175" y2="5.08" width="0.1524" layer="94" curve="180"/>
<wire x1="-0.635" y1="16.51" x2="-0.635" y2="3.81" width="0.635" layer="94"/>
<wire x1="0.635" y1="16.51" x2="0.635" y2="3.81" width="0.635" layer="94"/>
<wire x1="3.175" y1="10.16" x2="3.175" y2="12.7" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="12.7" x2="3.175" y2="15.24" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="15.24" x2="3.175" y2="17.78" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="7.62" x2="3.175" y2="10.16" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="5.08" x2="3.175" y2="7.62" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="2.54" x2="3.175" y2="5.08" width="0.1524" layer="94" curve="-180"/>
<wire x1="19.05" y1="6.985" x2="16.51" y2="6.985" width="0.1524" layer="94" curve="180"/>
<wire x1="16.51" y1="6.985" x2="13.97" y2="6.985" width="0.1524" layer="94" curve="180"/>
<wire x1="13.97" y1="6.985" x2="11.43" y2="6.985" width="0.1524" layer="94" curve="180"/>
<wire x1="19.05" y1="13.335" x2="16.51" y2="13.335" width="0.1524" layer="94" curve="-180"/>
<wire x1="16.51" y1="13.335" x2="13.97" y2="13.335" width="0.1524" layer="94" curve="-180"/>
<wire x1="13.97" y1="13.335" x2="11.43" y2="13.335" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="17.78" x2="11.43" y2="17.78" width="0.1524" layer="94"/>
<wire x1="11.43" y1="17.78" x2="11.43" y2="13.335" width="0.1524" layer="94"/>
<wire x1="11.43" y1="2.54" x2="3.175" y2="2.54" width="0.1524" layer="94"/>
<wire x1="11.43" y1="2.54" x2="11.43" y2="6.985" width="0.1524" layer="94"/>
<wire x1="19.05" y1="13.335" x2="19.05" y2="17.78" width="0.1524" layer="94"/>
<wire x1="19.05" y1="17.78" x2="21.59" y2="17.78" width="0.1524" layer="94"/>
<wire x1="19.05" y1="6.985" x2="19.05" y2="2.54" width="0.1524" layer="94"/>
<wire x1="19.05" y1="2.54" x2="21.59" y2="2.54" width="0.1524" layer="94"/>
<wire x1="11.43" y1="9.525" x2="19.05" y2="9.525" width="0.635" layer="94"/>
<wire x1="11.43" y1="10.795" x2="19.05" y2="10.795" width="0.635" layer="94"/>
<wire x1="-5.08" y1="-17.78" x2="-3.175" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="-10.16" x2="-3.175" y2="-10.16" width="0.1524" layer="94"/>
<wire x1="-5.08" y1="-2.54" x2="-3.175" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="-3.175" y1="-10.16" x2="-3.175" y2="-7.62" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="-7.62" x2="-3.175" y2="-5.08" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="-5.08" x2="-3.175" y2="-2.54" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="-12.7" x2="-3.175" y2="-10.16" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="-15.24" x2="-3.175" y2="-12.7" width="0.1524" layer="94" curve="180"/>
<wire x1="-3.175" y1="-17.78" x2="-3.175" y2="-15.24" width="0.1524" layer="94" curve="180"/>
<wire x1="-0.635" y1="-3.81" x2="-0.635" y2="-16.51" width="0.635" layer="94"/>
<wire x1="0.635" y1="-3.81" x2="0.635" y2="-16.51" width="0.635" layer="94"/>
<wire x1="3.175" y1="-10.16" x2="3.175" y2="-7.62" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-7.62" x2="3.175" y2="-5.08" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-5.08" x2="3.175" y2="-2.54" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-12.7" x2="3.175" y2="-10.16" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-15.24" x2="3.175" y2="-12.7" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-17.78" x2="3.175" y2="-15.24" width="0.1524" layer="94" curve="-180"/>
<wire x1="19.05" y1="-13.335" x2="16.51" y2="-13.335" width="0.1524" layer="94" curve="180"/>
<wire x1="16.51" y1="-13.335" x2="13.97" y2="-13.335" width="0.1524" layer="94" curve="180"/>
<wire x1="13.97" y1="-13.335" x2="11.43" y2="-13.335" width="0.1524" layer="94" curve="180"/>
<wire x1="19.05" y1="-6.985" x2="16.51" y2="-6.985" width="0.1524" layer="94" curve="-180"/>
<wire x1="16.51" y1="-6.985" x2="13.97" y2="-6.985" width="0.1524" layer="94" curve="-180"/>
<wire x1="13.97" y1="-6.985" x2="11.43" y2="-6.985" width="0.1524" layer="94" curve="-180"/>
<wire x1="3.175" y1="-2.54" x2="11.43" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="11.43" y1="-2.54" x2="11.43" y2="-6.985" width="0.1524" layer="94"/>
<wire x1="11.43" y1="-17.78" x2="3.175" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="11.43" y1="-17.78" x2="11.43" y2="-13.335" width="0.1524" layer="94"/>
<wire x1="19.05" y1="-6.985" x2="19.05" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="19.05" y1="-2.54" x2="21.59" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="19.05" y1="-13.335" x2="19.05" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="19.05" y1="-17.78" x2="21.59" y2="-17.78" width="0.1524" layer="94"/>
<wire x1="11.43" y1="-10.795" x2="19.05" y2="-10.795" width="0.635" layer="94"/>
<wire x1="11.43" y1="-9.525" x2="19.05" y2="-9.525" width="0.635" layer="94"/>
<wire x1="21.59" y1="-22.86" x2="20.32" y2="-22.86" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-22.86" x2="20.32" y2="-24.13" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-24.13" x2="20.32" y2="-25.4" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-25.4" x2="21.59" y2="-25.4" width="0.1524" layer="94"/>
<wire x1="21.59" y1="-30.48" x2="20.32" y2="-30.48" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-30.48" x2="20.32" y2="-31.75" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-31.75" x2="20.32" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-33.02" x2="21.59" y2="-33.02" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-24.13" x2="17.78" y2="-24.13" width="0.1524" layer="94"/>
<wire x1="17.78" y1="-24.13" x2="17.78" y2="-31.75" width="0.1524" layer="94"/>
<wire x1="17.78" y1="-31.75" x2="20.32" y2="-31.75" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="20.32" x2="-12.7" y2="-35.56" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-35.56" x2="-12.7" y2="-40.64" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-40.64" x2="-12.7" y2="-45.72" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-45.72" x2="-12.7" y2="-50.8" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-50.8" x2="-12.7" y2="-53.34" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-53.34" x2="22.86" y2="-53.34" width="0.254" layer="94"/>
<wire x1="-12.7" y1="20.32" x2="22.86" y2="20.32" width="0.254" layer="94"/>
<wire x1="22.86" y1="20.32" x2="22.86" y2="19.05" width="0.254" layer="94"/>
<wire x1="22.86" y1="-53.34" x2="22.86" y2="-34.29" width="0.254" layer="94"/>
<wire x1="22.86" y1="-29.21" x2="22.86" y2="-26.67" width="0.254" layer="94"/>
<wire x1="22.86" y1="-21.59" x2="22.86" y2="-19.05" width="0.254" layer="94"/>
<wire x1="22.86" y1="-16.51" x2="22.86" y2="-3.81" width="0.254" layer="94"/>
<wire x1="22.86" y1="-1.27" x2="22.86" y2="1.27" width="0.254" layer="94"/>
<wire x1="22.86" y1="3.81" x2="22.86" y2="16.51" width="0.254" layer="94"/>
<wire x1="-12.7" y1="-35.56" x2="20.32" y2="-35.56" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-35.56" x2="20.32" y2="-36.83" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="-40.64" x2="20.32" y2="-40.64" width="0.1524" layer="94"/>
<wire x1="21.59" y1="-46.99" x2="20.32" y2="-49.53" width="0.254" layer="94"/>
<wire x1="20.32" y1="-49.53" x2="19.05" y2="-46.99" width="0.254" layer="94"/>
<wire x1="21.59" y1="-49.53" x2="20.32" y2="-49.53" width="0.254" layer="94"/>
<wire x1="20.32" y1="-49.53" x2="19.05" y2="-49.53" width="0.254" layer="94"/>
<wire x1="21.59" y1="-46.99" x2="20.32" y2="-46.99" width="0.254" layer="94"/>
<wire x1="20.32" y1="-46.99" x2="19.05" y2="-46.99" width="0.254" layer="94"/>
<wire x1="20.32" y1="-46.99" x2="20.32" y2="-49.53" width="0.1524" layer="94"/>
<wire x1="18.288" y1="-47.752" x2="16.891" y2="-49.149" width="0.1524" layer="94"/>
<wire x1="18.415" y1="-48.895" x2="17.018" y2="-50.292" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-50.8" x2="20.32" y2="-49.53" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="-45.72" x2="20.32" y2="-45.72" width="0.1524" layer="94"/>
<wire x1="20.32" y1="-45.72" x2="20.32" y2="-46.99" width="0.1524" layer="94"/>
<wire x1="-12.7" y1="-50.8" x2="20.32" y2="-50.8" width="0.1524" layer="94"/>
<circle x="-3.175" y="16.51" radius="0.381" width="0" layer="94"/>
<circle x="-3.175" y="8.89" radius="0.381" width="0" layer="94"/>
<circle x="3.175" y="16.51" radius="0.381" width="0" layer="94"/>
<circle x="3.175" y="8.89" radius="0.381" width="0" layer="94"/>
<circle x="12.7" y="6.985" radius="0.381" width="0" layer="94"/>
<circle x="12.7" y="13.335" radius="0.381" width="0" layer="94"/>
<circle x="-3.175" y="-3.81" radius="0.381" width="0" layer="94"/>
<circle x="-3.175" y="-11.43" radius="0.381" width="0" layer="94"/>
<circle x="3.175" y="-3.81" radius="0.381" width="0" layer="94"/>
<circle x="3.175" y="-11.43" radius="0.381" width="0" layer="94"/>
<circle x="12.7" y="-13.335" radius="0.381" width="0" layer="94"/>
<circle x="12.7" y="-6.985" radius="0.381" width="0" layer="94"/>
<circle x="22.225" y="17.78" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="2.54" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-2.54" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-17.78" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-22.86" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-25.4" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-30.48" radius="0.635" width="0.1524" layer="94"/>
<circle x="22.225" y="-33.02" radius="0.635" width="0.1524" layer="94"/>
<circle x="20.32" y="-24.13" radius="0.381" width="0" layer="94"/>
<circle x="20.32" y="-31.75" radius="0.381" width="0" layer="94"/>
<text x="2.54" y="-38.1" size="1.778" layer="94">Right LED</text>
<text x="2.54" y="-48.26" size="1.778" layer="94">Left LED</text>
<text x="24.13" y="17.018" size="1.524" layer="94">1 TX+</text>
<text x="24.13" y="1.778" size="1.524" layer="94">2 TX-</text>
<text x="24.13" y="-3.302" size="1.524" layer="94">3 RX+</text>
<text x="24.13" y="-18.542" size="1.524" layer="94">6 RX-</text>
<text x="24.13" y="-23.622" size="1.524" layer="94">4</text>
<text x="24.13" y="-26.162" size="1.524" layer="94">5</text>
<text x="24.13" y="-31.242" size="1.524" layer="94">7</text>
<text x="24.13" y="-33.782" size="1.524" layer="94">8</text>
<text x="-12.7" y="21.59" size="1.778" layer="95">&gt;NAME</text>
<text x="-12.7" y="-55.88" size="1.778" layer="96">&gt;VALUE</text>
<pin name="C-R" x="-15.24" y="-40.64" visible="pad" length="short" direction="pas"/>
<pin name="A-R" x="-15.24" y="-35.56" visible="pad" length="short" direction="pas"/>
<pin name="C-L" x="-15.24" y="-50.8" visible="pad" length="short" direction="pas"/>
<pin name="A-L" x="-15.24" y="-45.72" visible="pad" length="short" direction="pas"/>
<pin name="TD+" x="-15.24" y="17.78" length="short" direction="in"/>
<pin name="CT" x="-15.24" y="10.16" length="short" direction="in"/>
<pin name="TD-" x="-15.24" y="2.54" length="short" direction="in"/>
<pin name="RD+" x="-15.24" y="-2.54" length="short" direction="in"/>
<pin name="CR" x="-15.24" y="-10.16" length="short" direction="in"/>
<pin name="RD-" x="-15.24" y="-17.78" length="short" direction="in"/>
<pin name="NC/4" x="-15.24" y="-25.4" length="short" direction="in"/>
<pin name="NC/5" x="-15.24" y="-30.48" length="short" direction="in"/>
<polygon width="0.1524" layer="94">
<vertex x="16.891" y="-38.989"/>
<vertex x="17.272" y="-38.1"/>
<vertex x="17.78" y="-38.608"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="17.018" y="-40.132"/>
<vertex x="17.399" y="-39.243"/>
<vertex x="17.907" y="-39.751"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="16.891" y="-49.149"/>
<vertex x="17.272" y="-48.26"/>
<vertex x="17.78" y="-48.768"/>
</polygon>
<polygon width="0.1524" layer="94">
<vertex x="17.018" y="-50.292"/>
<vertex x="17.399" y="-49.403"/>
<vertex x="17.907" y="-49.911"/>
</polygon>
</symbol>
</symbols>
<devicesets>
<deviceset name="J1006F01P" prefix="X">
<description>&lt;b&gt;PULSEJACK (TM) 1x1 Tab-UP RJ45&lt;/b&gt;&lt;p&gt;
10/100 Base-TX RJ45 1x1 Tab-UP with LEDs 8-pin (J1 series) and 6-pin (JP series) integrated magnetics connector,&lt;br&gt;
designed to support applications, such as ADSL modems, LAN-on-Motherboard, and Hub and Switches.&lt;br&gt;
Source: www.pulseeng.com .. PulseJack-J402.pdf</description>
<gates>
<gate name="G$1" symbol="J006-LED" x="0" y="0" addlevel="always"/>
</gates>
<devices>
<device name="" package="J1">
<connects>
<connect gate="G$1" pin="A-L" pad="12"/>
<connect gate="G$1" pin="A-R" pad="9"/>
<connect gate="G$1" pin="C-L" pad="11"/>
<connect gate="G$1" pin="C-R" pad="10"/>
<connect gate="G$1" pin="CR" pad="6"/>
<connect gate="G$1" pin="CT" pad="3"/>
<connect gate="G$1" pin="NC/4" pad="4"/>
<connect gate="G$1" pin="NC/5" pad="5"/>
<connect gate="G$1" pin="RD+" pad="7"/>
<connect gate="G$1" pin="RD-" pad="8"/>
<connect gate="G$1" pin="TD+" pad="1"/>
<connect gate="G$1" pin="TD-" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="pinhead">
<description>&lt;b&gt;Pin Header Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="1X02">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="1.016" diameter="2.1844" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="0" drill="1.016" diameter="2.1844" shape="long" rot="R90"/>
<text x="-2.6162" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
</package>
<package name="1X02/90">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="0" y1="-1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.27" y2="1.27" width="0.762" layer="21"/>
<pad name="1" x="-1.27" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="1.27" y="-3.81" drill="1.016" shape="long" rot="R90"/>
<text x="-3.175" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.651" y1="0.635" x2="-0.889" y2="1.143" layer="21"/>
<rectangle x1="0.889" y1="0.635" x2="1.651" y2="1.143" layer="21"/>
<rectangle x1="-1.651" y1="-2.921" x2="-0.889" y2="-1.905" layer="21"/>
<rectangle x1="0.889" y1="-2.921" x2="1.651" y2="-1.905" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="PINHD2">
<wire x1="-6.35" y1="-2.54" x2="1.27" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="1.27" y2="5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-2.54" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-1X2" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="PINHD2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X02">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="/90" package="1X02/90">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="X1" library="con-pulse" deviceset="J1006F01P" device=""/>
<part name="JP1" library="pinhead" deviceset="PINHD-1X2" device=""/>
<part name="JP2" library="pinhead" deviceset="PINHD-1X2" device=""/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="GND2" library="supply1" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="X1" gate="G$1" x="35.56" y="58.42"/>
<instance part="JP1" gate="G$1" x="-12.7" y="71.12" rot="R180"/>
<instance part="JP2" gate="G$1" x="-12.7" y="55.88" rot="R180"/>
<instance part="GND1" gate="1" x="-5.08" y="35.56"/>
<instance part="GND2" gate="1" x="5.08" y="73.66"/>
</instances>
<busses>
</busses>
<nets>
<net name="12V" class="0">
<segment>
<wire x1="12.7" y1="68.58" x2="12.7" y2="33.02" width="0.1524" layer="91"/>
<pinref part="X1" gate="G$1" pin="NC/4"/>
<wire x1="12.7" y1="33.02" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
<wire x1="12.7" y1="33.02" x2="12.7" y2="27.94" width="0.1524" layer="91"/>
<junction x="12.7" y="33.02"/>
<pinref part="X1" gate="G$1" pin="NC/5"/>
<wire x1="12.7" y1="27.94" x2="20.32" y2="27.94" width="0.1524" layer="91"/>
<pinref part="JP1" gate="G$1" pin="1"/>
<wire x1="12.7" y1="68.58" x2="-10.16" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="TTL" class="0">
<segment>
<pinref part="X1" gate="G$1" pin="RD-"/>
<wire x1="20.32" y1="40.64" x2="0" y2="40.64" width="0.1524" layer="91"/>
<wire x1="0" y1="40.64" x2="0" y2="55.88" width="0.1524" layer="91"/>
<pinref part="JP2" gate="G$1" pin="2"/>
<wire x1="0" y1="55.88" x2="-10.16" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="JP2" gate="G$1" pin="1"/>
<wire x1="-10.16" y1="53.34" x2="-5.08" y2="53.34" width="0.1524" layer="91"/>
<wire x1="-5.08" y1="53.34" x2="-5.08" y2="38.1" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
</segment>
<segment>
<pinref part="X1" gate="G$1" pin="TD+"/>
<wire x1="20.32" y1="76.2" x2="17.78" y2="76.2" width="0.1524" layer="91"/>
<wire x1="17.78" y1="76.2" x2="5.08" y2="76.2" width="0.1524" layer="91"/>
<wire x1="5.08" y1="76.2" x2="-2.54" y2="76.2" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="76.2" x2="-2.54" y2="71.12" width="0.1524" layer="91"/>
<pinref part="X1" gate="G$1" pin="TD-"/>
<wire x1="20.32" y1="60.96" x2="17.78" y2="60.96" width="0.1524" layer="91"/>
<wire x1="17.78" y1="60.96" x2="17.78" y2="76.2" width="0.1524" layer="91"/>
<junction x="17.78" y="76.2"/>
<pinref part="JP1" gate="G$1" pin="2"/>
<wire x1="-10.16" y1="71.12" x2="-2.54" y2="71.12" width="0.1524" layer="91"/>
<pinref part="GND2" gate="1" pin="GND"/>
<junction x="5.08" y="76.2"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
