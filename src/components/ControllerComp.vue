<template>
  <div class="inputDiv">
    <!-- Titel -->
    <h3 class="title">{{ props.name }}</h3>

    <!-- Numberboxar -->
    <div class="numberBox-section">
      <label class="labelHead">{{ props.labelHead }}</label>

      <!-- Temperatur -->
      <div v-if="turnOnAtTempValue !== null" class="number-box">
        <q-input color="green-10"
          v-model.number="turnOnAtTempValue"
          type="number"
          min="1"
          max="100"
          @update:model-value="logTempTurnOn"
          label="Temperatur °C"
        />
      </div>

      <!-- Luftfuktighet -->
      <div v-if="turnOnAtHumValue !== null" class="number-box">
        <q-input color="green-10"
          v-model.number="turnOnAtHumValue"
          type="number"
          min="1"
          max="100"
          @update:model-value="logHumTurnOn"
          label="Luftfuktighet %"
        />
      </div>
    </div>

    <!-- Sliders -->
    <div class="slider-section">
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
      <br/>
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
  </div>
</template>

<script setup>
import { ref, watch } from 'vue'

// Props
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
.title{
  margin: 6%;
  padding: 0%;
  font-size: 34px;
  font-weight: bold;
}

.labels {
  margin: 0%;
  padding: 0%;
  font-size: 20px; 
}

.labelHead {
  margin: 0%;
  padding: 0%;
  margin-top: 5%;
  font-size: 20px;
  font-weight: bold;
}

.numberBox-section {
  margin-top: 10%;
  display: flex;
  flex-direction: column;
}

.number-box {
  margin-bottom: 0%;
  flex: 1;
  min-width: 20%;
}

.slider-section {
  margin-top: 14%;
}

.checkbox {
  color: red;
  transition-duration: 0.2ms;
}
</style>
