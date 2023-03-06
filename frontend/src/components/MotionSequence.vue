<script lang="ts" setup>
import { defineProps, reactive, toRefs } from "vue";
import { MotionSequence } from "@/Api";
const props = defineProps<{
   sequence: MotionSequence;
}>();
const { sequence } = toRefs(props);
</script>

<template>
   <v-sheet border rounded>
      <v-row
         ><v-col cols="12" sm="6" md="4">
            <v-text-field
               v-model="sequence.name"
               clearable
               density="compact"
               hide-details="auto"
               label="Name"
            ></v-text-field>
         </v-col>
         <v-col cols="12" sm="6" md="4">
            <v-checkbox
               color="primary"
               label="Loop"
               v-model="sequence.loop"
            ></v-checkbox>
         </v-col>
      </v-row>
      <v-sheet border rounded>
         lala
         <v-row> </v-row>
      </v-sheet>
   </v-sheet>
</template>
