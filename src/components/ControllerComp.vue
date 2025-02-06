<template>
  <div class="inputDiv">
    <!-- Titel -->
    <h3>{{ props.name }}</h3>

    <!--Numberboxar -->
    <div class="numberBox-section">
<<<<<<< HEAD
      <label class="labelHead">{{ props.labelHead }}</label> 
      <br/>
      <input
=======
      <label>{{ props.labelHead }}</label
      ><br />
      <q-input
>>>>>>> 25aa695c0917e1b766d40757dd4a8d102ab359c2
        v-if="turnOnAtTempValue !== null"
        v-model.number="turnOnAtTempValue"
        type="number"
        min="1"
        max="100"
        filled
        @update:model-value="logTempTurnOn"
      />
      <label class="labels">{{ props.labelTemp }}</label>

      <q-input
        v-if="turnOnAtHumValue !== null"
        v-model.number="turnOnAtHumValue"
        type="number"
        min="1"
        max="100"
<<<<<<< HEAD
        v-model="turnOnAtHumValue"
      />
      <label class="labels">{{ props.labelHum }}</label>
=======
        @update:model-value="logHumTurnOn"
      ></q-input>
      <label>{{ props.labelHum }}</label>
>>>>>>> 25aa695c0917e1b766d40757dd4a8d102ab359c2
    </div>

    <!-- Sliders -->
    <div class="slider-section"></div>
    <label class="labels">{{ props.labelMain }}</label>
    <br/>
    <label class="labels">{{ inputValue }}</label>
    <q-slider
      v-if="inputValue !== null"
      @change="logManualSpeed"
      v-model.lazy.number="inputValue"
      :min="1"
      :max="100"
      :step="1"
      color="green-10"
    />
  </div>

  <!-- Checkboxar -->

  <div class="checkBox-section">
    <label class="labels">{{ props.labelManOve }}</label>
    <q-toggle
      size="xl"
      class="checkbox"
      color="green-10"
      v-model="manualOverrideValue"
      @update:model-value="logManualOverride"
    />
    <label class="labels">{{ props.labelManOn }}</label>
    <q-toggle
      size="xl"
      class="checkbox"
      color="green-10"
      v-model="manualOnValue"
      :disable="!manualOverrideValue"
      @update:model-value="logManualOn"
    />
  </div>
</template>

<script setup>
import { ref, watch } from 'vue'

//Props
const props = defineProps({
  name: String,
  isOnValue: Boolean,
  inputValue: Number,
  manualOverrideValue: Boolean,
  turnOnAtHumValue: Number,
  turnOnAtTempValue: {
    type: Number,
    default: null,
  },
  openValue: {
    type: Boolean,
    default: null,
  },
  manualOn: Boolean,
  labelTemp: String,
  labelHum: String,
  labelMain: String,
  labelManOve: String,
  labelManOn: String,
  labelHead: String,
})

watch(
  () => props.startupValInt,
  (newValue) => {
    inputValue.value = newValue
  },
)
watch(
  () => props.startupValBool,
  (newValue) => {
    manualOverrideValue.value = newValue
  },
)
watch(
  () => props.turnOnAtHum,
  (newValue) => {
    manualOverrideValue.value = newValue
  },
)

const isOnValue = ref(props.isOnValue)
const inputValue = ref(props.inputValue)
const manualOverrideValue = ref(props.manualOverrideValue)
const turnOnAtHumValue = ref(props.turnOnAtHumValue)
const turnOnAtTempValue = ref(props.turnOnAtTempValue)
const openValue = ref(props.openValue)
const manualOnValue = ref(props.manualOn)

const emits = defineEmits([
  'updateManualOverride',
  'updateManualSpeed',
  'updateTurnOnAtTemp',
  'updateTurnOnAtHum',
  'updateManualOn',
  'updateManualOpen',
])

function logManualSpeed() {
  emits('updateManualSpeed', inputValue.value)
}
function logManualOverride() {
  emits('updateManualOverride', manualOverrideValue.value)
}
function logTempTurnOn() {
  emits('updateTurnOnAtTemp', turnOnAtTempValue.value)
}
function logHumTurnOn() {
  emits('updateTurnOnAtHum', turnOnAtHumValue.value)
}
function logManualOn() {
  emits('updateManualOn', manualOnValue.value)
}

watch(
  () => props,
  (newProps) => {
    isOnValue.value = newProps.isOnValue
    inputValue.value = newProps.inputValue
    manualOverrideValue.value = newProps.manualOverrideValue
    turnOnAtHumValue.value = newProps.turnOnAtHumValue
    turnOnAtTempValue.value = newProps.turnOnAtTempValue
    openValue.value = newProps.openValue
  },
  { deep: true },
)

watch(inputValue, (val) => {
  if (val < 0 || val === isNaN() || val === undefined) {
    inputValue.value = 0
  } else if (val > 100) {
    inputValue.value = 100
  }
})
</script>

<style>
.labels {
  margin: 0;
  padding: 0;
  font-size: 20px; 
}

.labelHead {
  margin: 0;
  padding: 0;
  font-size: 20px;
  font-weight: bold;
}


input[type='number'] {
  background-color: #dedede;
  margin: 5%;
  padding: 5%;
  border-radius: 2px;
  border-style: solid;
  size: 100%;
}

.checkbox {
  color: red;
  transition-duration: 0.2ms;
}
</style>
