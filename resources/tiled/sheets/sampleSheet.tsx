<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.10" tiledversion="1.11.2" name="tileset" tilewidth="16" tileheight="16" spacing="1" tilecount="132" columns="12">
 <image source="../tilemaps/tilemap.png" width="203" height="186"/>
 <tile id="0">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="1">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="2">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="3">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="4">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="5">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="6">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="12">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="13">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="15">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="16">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="17">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="24">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="25">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="26">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="27">
  <properties>
   <property name="collide" type="bool" value="true"/>
  </properties>
  <objectgroup draworder="index" id="2">
   <object id="1" x="0" y="0" width="16" height="16"/>
   <object id="3" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <wangsets>
  <wangset name="Pit" type="mixed" tile="-1">
   <wangcolor name="pit walls" color="#ff0000" tile="-1" probability="1"/>
   <wangcolor name="pit ground" color="#00ff00" tile="-1" probability="1"/>
   <wangtile tileid="1" wangid="0,0,0,1,0,0,0,0"/>
   <wangtile tileid="2" wangid="0,0,0,1,1,1,0,0"/>
   <wangtile tileid="3" wangid="0,0,0,0,0,1,0,0"/>
   <wangtile tileid="13" wangid="0,1,1,1,0,0,0,0"/>
   <wangtile tileid="14" wangid="1,1,1,1,2,1,1,1"/>
   <wangtile tileid="15" wangid="0,0,0,0,0,1,1,1"/>
   <wangtile tileid="25" wangid="0,1,0,0,0,0,0,0"/>
   <wangtile tileid="26" wangid="2,1,0,0,0,0,0,1"/>
   <wangtile tileid="27" wangid="0,0,0,0,0,0,0,1"/>
   <wangtile tileid="48" wangid="2,1,1,1,2,1,1,1"/>
  </wangset>
  <wangset name="dirt" type="mixed" tile="-1">
   <wangcolor name="dirt" color="#ff0000" tile="-1" probability="1"/>
   <wangcolor name="wall" color="#00ff00" tile="-1" probability="1"/>
   <wangtile tileid="16" wangid="0,0,0,2,2,2,2,2"/>
   <wangtile tileid="17" wangid="0,2,2,2,2,2,0,0"/>
   <wangtile tileid="28" wangid="2,2,2,2,2,2,2,2"/>
   <wangtile tileid="40" wangid="2,2,2,2,2,2,2,2"/>
   <wangtile tileid="42" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="48" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="49" wangid="1,1,1,1,1,1,1,1"/>
   <wangtile tileid="50" wangid="2,2,1,1,1,1,1,2"/>
   <wangtile tileid="51" wangid="2,2,1,1,1,1,1,2"/>
   <wangtile tileid="52" wangid="2,2,2,2,1,1,1,2"/>
   <wangtile tileid="53" wangid="1,1,1,1,1,1,1,2"/>
   <wangtile tileid="57" wangid="2,2,2,2,2,1,1,1"/>
   <wangtile tileid="58" wangid="2,1,1,1,2,1,1,1"/>
   <wangtile tileid="59" wangid="2,1,1,1,2,2,2,2"/>
  </wangset>
 </wangsets>
</tileset>
