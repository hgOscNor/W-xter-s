<template>
  <div class="inputDiv">
    <!-- namn på comp -->
    <h3>{{ props.name }}</h3>

    <button @click="LogData">Log</button>

    <!-- turn on at ... -->
    <input type="number" min="0" max="100" v-model="humTurnOnvalue" />
    <input type="number" min="0" max="100" v-model="tempTurnOnvalue" />
    <!-- speed -->
    <input @update:model-value="LogData" type="number" min="0" max="100" v-model="inputValue" />

    <!-- speed -->
    <div>
      <q-slider
        @update:model-value="LogData"
        v-model.lazy.number="inputValue"
        :min="0"
        :max="100"
        :step="1"
      />
    </div>
    <!--Manual override  -->
    <q-toggle
      size="xl"
      class="checkbox"
      v-model="manualOverrideValue"
      @input="emit('update:manualOverride', $event.target.value)"
    />
    <!-- Absolut??? -->
    <q-toggle size="xl" class="checkbox" v-model="manualOverrideValue" />
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
  turnOnAtTempValue: Number,
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

// Emits
// const emit = defineEmits([
//   'update:isOn',
//   'update:startupValInt',
//   'update:manualOverrride',
//   'update:turnOnAtHum',
//   'update:turnOnAtTemp',
// ])

const emits = defineEmits(['log'])

function LogData() {
  emits('log', inputValue.value)
}

watch(
  () => props,
  (newProps) => {
    isOnValue.value = newProps.isOnValue
    inputValue.value = newProps.inputValue
    manualOverrideValue.value = newProps.manualOverrideValue
    turnOnAtHumValue.value = newProps.turnOnAtHumValue
    turnOnAtTempValue.value = newProps.turnOnAtTempValue
  },
  { deep: true },
)

// S(inputValue, (newValue) => emit('manualSpeed', newValue))

watch(manualOverrideValue, (val) => {
  console.log(val)
})

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
  background-color: green;
}
</style>
