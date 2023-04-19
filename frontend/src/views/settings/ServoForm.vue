<template>
   <v-row>
      <v-col
         ><v-text-field
            v-model="value.id"
            label="id"
            type="number"
            :rules="[isRequired('id'), isGreaterEqualValue('id', 0)]"
      /></v-col>
      <v-col><v-text-field type="text" label="Servo Name" /></v-col>
   </v-row>
   <v-row>
      <v-col>
         <v-select
            v-model="value.pin"
            label="Select a pin"
            :items="PWM_PINS"
            item-props="value"
            item-title="label"
            return-object
      /></v-col>
      <v-col
         ><v-text-field
            v-model="value.position"
            label="Start Position in°"
            :rules="[
               isRequired('Start Position'),
               isGreaterEqualValue('Start Position', 0),
               isSmallerEqualValue('Start Position', 360),
            ]"
            type="number"
      /></v-col>
   </v-row>
   <v-row>
      <v-col
         ><v-text-field
            v-model="value.minPwm"
            type="number"
            label="Min PWM"
            :rules="[
               isRequired('Min PWM'),
               isGreaterEqualValue('Min PWM', 0),
               isSmallerEqualValue('Min PWM', value.maxPwm),
            ]"
      /></v-col>
      <v-col
         ><v-text-field
            v-model="value.maxPwm"
            type="number"
            label="Max PWM"
            :rules="[isRequired('Max PWM'), isGreaterEqualValue('Max PWM', 0)]"
         />
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
import { isRequired, isGreaterEqualValue, isSmallerEqualValue } from "@/form";
import { PWM_PINS } from "@/tools";
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
