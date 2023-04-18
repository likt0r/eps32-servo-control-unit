<template>
   <v-row>
      <v-col
         ><v-text-field v-model="value.id" label="id" type="number"
      /></v-col>
      <v-col><v-text-field type="text" label="Servo Name" /></v-col>
   </v-row>
   <v-row>
      <v-col
         ><v-text-field v-model="value.pin" type="number" label="Pin"
      /></v-col>
      <v-col
         ><v-text-field
            v-model="value.position"
            label="Start Position in°"
            type="number"
      /></v-col>
   </v-row>
   <v-row>
      <v-col
         ><v-text-field v-model="value.minPwm" type="number" label="Min PWM"
      /></v-col>
      <v-col
         ><v-text-field v-model="value.maxPwm" type="number" label="Max PWM" />
      </v-col>
   </v-row>
   <v-row>
      <v-range-slider
         label="Angle Range in°"
         v-model="angleRange"
         step="1"
         min="0"
         max="360"
         thumb-label="always"
      />
   </v-row>
</template>
<script setup lang="ts">
import { computed } from "vue";
import { Servo } from "@/ApiService";

export interface Props {
   modelValue: Servo;
}

const props = defineProps<Props>();
const angleRange = computed({
   get() {
      return [props.modelValue.minAngle, props.modelValue.maxAngle];
   },
   set(value) {
      props.modelValue.minAngle = value[0];
      props.modelValue.maxAngle = value[1];
   },
});

const emit = defineEmits<{
   (e: "update:modelValue", value: Servo): void;
}>();

const value = computed({
   get() {
      return props.modelValue;
   },
   set(value) {
      emit("update:modelValue", value);
   },
});
</script>
