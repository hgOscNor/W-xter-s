<template>
  <div class="controllerObj">
    <ControllerComp
      name="Fläktkontroller"
      :isOnValue="fanData?.isOn || false"
      :manualOverrideValue="fanData?.manualOverride || false"
      :inputValue="fanData?.manualSpeed || 0"
      :turnOnAtHumValue="fanData?.turnOnAtHum || 0"
      :turnOnAtTempValue="fanData?.turnOnAtTemp || 0"
      @update:manualOverridevalue="fanDataUpdate"
      @updateManualOverride="logManualOverrideFan"
      @updateManualSpeed="logManualSpeedFan"
    ></ControllerComp>
    <ControllerComp name="Pump"></ControllerComp>
  </div>
</template>

<script setup>
import ControllerComp from '../components/ControllerComp.vue'
import { /*ref as vueRef*/ /*onMounted,*/ watch } from 'vue'

//Firebase
import {} from /*useRouter*/ 'vue-router'
import { useDatabaseObject } from 'vuefire'
import { ref as dbRef, set } from 'firebase/database'
import { db } from 'src/boot/vuefire'
// const router = useRouter()

// Ref
const fanManualOverrideRef = dbRef(db, 'control/fan/manualOverride')
const fanSpeedRef = dbRef(db, 'control/fan/manualSpeed')

function logManualOverrideFan(data) {
  set(fanManualOverrideRef, Boolean(data))
}

function logManualSpeedFan(data) {
  set(fanSpeedRef, Number(data))
  console.log(data)
}

const data = useDatabaseObject(dbRef(db, 'control'))
const fanData = defineModel({ default: 0 })
watch(data, (newData) => {
  if (newData && newData.fan) {
    fanData.value = { ...newData.fan }
  }
})
console.log(fanData.value)
console.log('test')

// Update firebase
function fanDataUpdate(newValue) {
  fanData.value.manualOverride = newValue
  console.log('Update:', newValue)
}
</script>

<style>
.controllerObj {
  display: flex;
  justify-content: center;
  flex-direction: column;
  align-items: center;
  flex-wrap: nowrap;
}
</style>
