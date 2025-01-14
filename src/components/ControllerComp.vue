<template>
  <div class="inputDiv">
    <h3>{{ props.name }}</h3>

    <input
      type="number"
      min="0"
      max="100"
      v-model="humTurnOn"
      @input="emitUpdate('turnOnAtHum', turnOnAtHum)"
    />
    <input type="number" min="0" max="100" v-model="tempTurnOn" />
    <input type="number" min="0" max="100" v-model="inputValue" />

    <div>
      <q-slider v-model.lazy.number="inputValue" :min="0" :max="100" :step="1" />
    </div>

    <q-toggle size="xl" class="checkbox" v-model="checkbox" />
  </div>
</template>

<script setup>
import { ref, watch } from 'vue'

//Props
const props = defineProps({
  name: String,
  isOn: Boolean,
  startupValInt: Number,
  startupValBool: Boolean,
  turnOnAtHum: Number,
  turnOnAtTemp: Number,
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
    checkbox.value = newValue
  },
)
watch(
  () => props.turnOnAtHum,
  (newValue) => {
    checkbox.value = newValue
  },
)

const inputValue = ref(props.startupValInt)
const checkbox = ref(props.startupValBool)
const humTurnOn = ref(props.turnOnAtHum)
const tempTurnOn = ref(props.turnOnAtTemp)

// Emits
const emit = defineEmits([
  'update:isOn',
  'update:startupValInt',
  'update:startupValBool',
  'update:turnOnAtHum',
  'update:turnOnAtTemp',
])

watch(inputValue, (newValue) => emit('update:startupValInt', newValue))

watch(checkbox, (val) => {
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

/* .inputDiv {
  display: flex;
  flex-direction: row;
  justify-content: center;
  align-items: center;
  gap: 10%;
} */
</style>
