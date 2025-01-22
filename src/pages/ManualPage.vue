<template>
  <div class="controllerObj">
    <div class="controllerBox">
      <ControllerComp
        name="Fläktkontroller"
        :isOnValue="fanData?.isOn || false"
        :manualOverrideValue="fanData?.manualOverride || false"
        :inputValue="fanData?.manualSpeed || 0"
        :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
        :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
        @updateManualOverride="logManualOverrideFan"
        @updateManualSpeed="logManualSpeedFan"
        @updateTurnOnAtTemp="logTurnOnTempFan"
        @updateTurnOnAtHum="logTurnOnHumFan"
      />
    </div>
    <div class="controllerBox">
      <ControllerComp
        name="Pumpkontroller"
        :isOnValue="pumpData?.isOn || false"
        :manualOverrideValue="pumpData?.manualOverride || false"
        :inputValue="pumpData?.manualSpeed || 0"
        :turnOnAtHumValue="pumpData?.turnOnAtSoil || 0"
        @updateManualOverride="logManualOverridePump"
        @updateManualSpeed="logManualSpeedPump"
        @updateTurnOnAtHum="logTurnOnSoilPump"
      />
    </div>
    <div class="controllerBox">
      <ControllerComp
        name="Luckkontroller"
        :isOnValue="trapdoorData?.isOn || false"
        :manualOverrideValue="trapdoorData?.manualOverride || false"
        :openValue="trapdoorData?.open || false"
        :turnOnAtHumValue="trapdoorData?.turnOnAtHum || 0"
        :turnOnAtTempValue="trapdoorData?.turnOnAtTemp || 0"
        :inputValue="null"
        @updateManualOverride="logManualOverrideTrapdoor"
        @updateManualSpeed="logManualOpenTrapdoor"
        @updateTurnOnAtTemp="logTurnOnTempTrapdoor"
        @updateTurnOnAtHum="logTurnOnHumTrapdoor"
      />
    </div>
    <div class="controllerBox">
      <ControllerComp
        name="Lampkontroller"
        :isOnValue="lightsData?.isOn || false"
        :manualOverrideValue="lightsData?.manualOverride || false"
        :turnOnAtHumValue="lightsData?.turnOnAt || 0"
        @updateManualOverride="logManualOverrideLights"
        @updateManualSpeed="logManualOpenLights"
        @updateTurnOnAtTemp="logTurnOnTempLights"
      />
    </div>
<<<<<<< HEAD
=======
=======
    <ControllerComp
      name="Fläkt kontroller"
      :isOnValue="fanData?.isOn || false"
      :manualOverrideValue="fanData?.manualOverride || false"
      :inputValue="fanData?.manualSpeed || 0"
      :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
      :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
      :manualOnValue="fan?.manualOn || false"
      @updateManualOverride="logManualOverrideFan"
      @updateManualSpeed="logManualSpeedFan"
      @updateTurnOnAtTemp="logTurnOnTempFan"
      @updateTurnOnAtHum="logTurnOnHumFan"
      @updateManualOn="logManualOnFan"
    ></ControllerComp>
    <ControllerComp
      name="Pump kontroller"
      :isOnValue="pumpData?.isOn || false"
      :manualOverrideValue="pumpData?.manualOverride || false"
      :inputValue="pumpData?.manualSpeed || 0"
      :turnOnAtHumValue="pumpData?.turnOnAtSoil || 0"
      :manualOnValue="pump?.manualOn || false"
      @updateManualOverride="logManualOverridePump"
      @updateManualSpeed="logManualSpeedPump"
      @updateTurnOnAtHum="logTurnOnSoilPump"
      @updateManualOn="logManualOnPump"
    ></ControllerComp>
    <ControllerComp
      name="Luck kontroller"
      :isOnValue="trapdoorData?.isOn || false"
      :manualOverrideValue="trapdoorData?.manualOverride || false"
      :openValue="trapdoorData?.open || false"
      :turnOnAtHumValue="trapdoorData?.turnOnAtHum || 0"
      :turnOnAtTempValue="trapdoorData?.turnOnAtTemp || 0"
      :manualOnValue="trapdoor?.manualOn || false"
      :inputValue="null"
      @updateManualOverride="logManualOverrideTrapdoor"
      @updateManualOpen="logManualOpenTrapdoor"
      @updateTurnOnAtTemp="logTurnOnTempTrapdoor"
      @updateTurnOnAtHum="logTurnOnHumTrapdoor"
      @updateManualOn="logManualOnTrapdoor"
    ></ControllerComp>
    <ControllerComp
      name="Lamp kontroller"
      :isOnValue="lights?.isOn || false"
      :manualOverrideValue="lights?.manualOverride || false"
      :turnOnAtHumValue="lights?.turnOnAt || 0"
      :manualOnValue="lights?.manualOn || false"
      @updateManualOverride="logManualOverrideLights"
      @updateManualSpeed="logDimmer"
      @updateTurnOnAtTemp="logTurnOnAtLights"
      @updateManualOn="logManualOnLights"
    ></ControllerComp>
>>>>>>> 64f03a59973dac0868283b645aefcc805554414c
>>>>>>> 1c47cedee43d53d8588c41930ba5025cd37bdd55
  </div>
</template>

<script setup>
import ControllerComp from "../components/ControllerComp.vue";
import { ref as vueRef, watch } from "vue";
import { useDatabaseObject } from "vuefire";
import { ref as dbRef, set } from "firebase/database";
import { db } from "src/boot/vuefire";

// Firebase Refs
const fanManualOverrideRef = dbRef(db, "control/fan/manualOverride");
const fanSpeedRef = dbRef(db, "control/fan/manualSpeed");
const fanTurnOnAtTempRef = dbRef(db, "control/fan/turnOnAtTemp");
const fanTurnOnAtHumRef = dbRef(db, "control/fan/turnOnAtHum");

// Firebase
//    Ref
const fanManualOverrideRef = dbRef(db, 'control/fan/manualOverride')
const fanSpeedRef = dbRef(db, 'control/fan/manualSpeed')
const fanTurnOnAtTempRef = dbRef(db, 'control/fan/turnOnAtTemp')
const fanTurnOnAtHumRef = dbRef(db, 'control/fan/turnOnAtHum')
const fanManualOnRef = dbRef(db, 'control/fan/manualOn')

const pumpManualOverrideRef = dbRef(db, 'control/pump/manualOverride')
const pumpSpeedRef = dbRef(db, 'control/pump/manualSpeed')
const pumpTurnOnAtSoilRef = dbRef(db, 'control/pump/turnOnAtSoil')
const pumpManualOnRef = dbRef(db, 'control/pump/manualOn')

const trapdoorManualOverrideRef = dbRef(db, 'control/trapdoor/manualOverride')
const trapdoorOpenRef = dbRef(db, 'control/trapdoor/open')
const trapdoorTurnOnAtTempRef = dbRef(db, 'control/trapdoor/openAtTemp')
const trapdoorTurnOnAtHumRef = dbRef(db, 'control/trapdoor/openAtHum')
const trapdoorManualOnRef = dbRef(db, 'control/trapdoor/manualOn')

const lightsManualOverrdideRef = dbRef(db, 'control/lights/manualOverride')
const lightsDimmerRef = dbRef(db, 'control/lights/dimmer')
const lightsTurnOnAtRef = dbRef(db, 'control/lights/turnOnAt')
const lightsManualOnRef = dbRef(db, 'control/lights/manualOn')

//  Write to firebase
//    Fan
function logManualOverrideFan(data) {
  set(fanManualOverrideRef, Boolean(data));
}
function logManualSpeedFan(data) {
  set(fanSpeedRef, Number(data));
}
function logTurnOnTempFan(data) {
  set(fanTurnOnAtTempRef, Number(data));
}
function logTurnOnHumFan(data) {
  set(fanTurnOnAtHumRef, Number(data));
}
<<<<<<< HEAD

=======
function logManualOnFan(data) {
  set(fanManualOnRef, Boolean(data))
}
//    Pump
>>>>>>> 1c47cedee43d53d8588c41930ba5025cd37bdd55
function logManualOverridePump(data) {
  set(pumpManualOverrideRef, Boolean(data));
}
function logManualSpeedPump(data) {
  set(pumpSpeedRef, Number(data));
}
function logTurnOnSoilPump(data) {
  set(pumpTurnOnAtSoilRef, Number(data));
}
<<<<<<< HEAD

=======
function logManualOnPump(data) {
  set(pumpManualOnRef, Boolean(data))
}
//    Trapdoor
>>>>>>> 1c47cedee43d53d8588c41930ba5025cd37bdd55
function logManualOverrideTrapdoor(data) {
  set(trapdoorManualOverrideRef, Boolean(data));
}
function logManualOpenTrapdoor(data) {
<<<<<<< HEAD
  set(trapdoorSpeedRef, Boolean(data));
=======
  set(trapdoorOpenRef, Boolean(data))
>>>>>>> 1c47cedee43d53d8588c41930ba5025cd37bdd55
}
function logTurnOnTempTrapdoor(data) {
  set(trapdoorTurnOnAtTempRef, Number(data));
}
function logTurnOnHumTrapdoor(data) {
  set(trapdoorTurnOnAtHumRef, Number(data));
}
<<<<<<< HEAD

function logManualOverrideLights(data) {
  set(fanManualOverrideRef, Boolean(data));
}

// Data from Firebase
const data = useDatabaseObject(dbRef(db, "control"));
const fanData = vueRef();
const pumpData = vueRef();
const trapdoorData = vueRef();
const lightsData = vueRef();
=======
function logManualOnTrapdoor(data) {
  set(trapdoorManualOnRef, Boolean(data))
}
//  Lights
function logManualOverrideLights(data) {
  set(lightsManualOverrdideRef, Boolean(data))
}
function logDimmer(data) {
  set(lightsDimmerRef, Number(data))
}
function logTurnOnAtLights(data) {
  set(lightsTurnOnAtRef, Number(data))
}
function logManualOnLights(data) {
  set(lightsManualOnRef, Boolean(data))
}

const data = useDatabaseObject(dbRef(db, 'control'))
const fanData = vueRef()
const pumpData = vueRef()
const trapdoorData = vueRef()
const lightsData = vueRef()
>>>>>>> 1c47cedee43d53d8588c41930ba5025cd37bdd55

watch(data, (newData) => {
  if (newData?.fan) fanData.value = { ...newData.fan };
  if (newData?.pump) pumpData.value = { ...newData.pump };
  if (newData?.trapdoor) trapdoorData.value = { ...newData.trapdoor };
  if (newData?.lights) lightsData.value = { ...newData.lights };
});
</script>

<style>
.controllerObj {
  display: flex;
  justify-content: space-around;
  align-items: flex-start;
  flex-wrap: wrap;
  gap: 0.1%;
  row-gap: 40px;
  padding: 2%;
}

.controllerBox {
  width: 40%; 
  height: 500px;
  padding: 10px;
  border-radius: 40px;
  border: 2px solid black;
  flex-direction: column;
  text-align: center;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

</style>

