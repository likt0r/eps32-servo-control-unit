<template>
   <v-row>
      <v-col
         ><v-text-field
            v-model="value.id"
            label="id"
            type="number"
            :rules="[isRequired('id'), isGreaterEqualValue('id', 0)]"
      /></v-col>
      <v-col><v-text-field type="text" label="Led Name" /></v-col>
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
         ><v-switch
            color="secondary"
            :label="`Led starts ${value.isOn ? 'on' : 'off'}`"
            v-model="value.isOn"
         ></v-switch>
      </v-col>
   </v-row>
</template>
<script setup lang="ts">
import { computed, ref } from "vue";
import { Led } from "@/ApiService";
import {
   isRequired,
   isGreaterEqualValue,
   isGreaterOtherFieldValue,
   isSmallerEqualValue,
} from "@/form";

import { PWM_PINS } from "@/tools";
export interface Props {
   modelValue: Led;
}

const props = defineProps<Props>();

const emit = defineEmits<{
   (e: "update:modelValue", value: Led): void;
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
