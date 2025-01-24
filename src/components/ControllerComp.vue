<template>
  <div class="inputDiv">
    <!-- namn på comp -->
    <h3>{{ props.name }}</h3>

    <!-- turn on at ... -->
    <input
      v-if="turnOnAtTempValue !== null"
      @update:model-value="logTempTurnOn"
      type="number"
      min="0"
      max="100"
      v-model="turnOnAtTempValue"
    />
    <input
      v-if="turnOnAtHumValue !== null"
      @update:model-value="logHumTurnOn"
      type="number"
      min="0"
      max="100"
      v-model="turnOnAtHumValue"
    />

    <!-- speed -->
    <h5>{{ inputValue }}</h5>
    <div>
      <q-slider
        v-if="inputValue !== null"
        @change="logManualSpeed"
        v-model.lazy.number="inputValue"
        :min="0"
        :max="100"
        :step="1"
      />
      <q-toggle
        v-else
        size="xl"
        class="checkbox"
        v-model="openValue"
        @update:model-value="logManualOpen"
      />
    </div>
    <!--Manual override  -->
    <q-toggle
      size="xl"
      class="checkbox"
      v-model="manualOverrideValue"
      @update:model-value="logManualOverride"
    />
    <!-- Absolut??? -->
    <q-toggle
      size="xl"
      class="checkbox"
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
function logManualOpen() {
  emits('updateManualOpen', openValue.value)
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

.inputDiv {
  /* display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  gap: 10%; */ 
} 
</style>
