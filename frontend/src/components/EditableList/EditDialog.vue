<template>
   <v-dialog v-model="dialog" :persistent="persistent" width="1024">
      <template v-slot:activator="{ props }">
         <v-btn
            :icon="buttonIcon"
            color="secondary"
            v-bind="props"
            variant="text"
            :disabled="disabled"
         ></v-btn>
      </template>
      <v-card>
         <v-form validate-on="submit" @submit.prevent="submit">
            <v-card-title>
               <span class="text-h5">{{ title }}</span>
            </v-card-title>
            <v-card-text>
               <component :is="formComponent" v-model="_data" />
            </v-card-text>
            <v-card-actions>
               <v-spacer></v-spacer>
               <v-btn @click="dialog = false"> Cancel </v-btn>
               <v-btn color="primary" type="submit"> Save </v-btn>
            </v-card-actions>
         </v-form>
      </v-card>
   </v-dialog>
</template>
<script setup lang="ts">
import { onBeforeUpdate } from "@vue/runtime-core";
import clone from "lodash/clone";
import { ref } from "vue";
import { SubmitEventPromise } from "vuetify/lib/framework.mjs";

export interface Props {
   data: any;
   persistent: boolean;
   buttonIcon: string;
   formComponent: any;
   title: string;
   disabled: boolean;
}

const props = defineProps<Props>();

const emit = defineEmits<{
   (e: "update:modelValue", value: any): void;
   (e: "cancel"): void;
   (e: "save", data: any): void;
}>();
const dialog = ref<boolean>(false);
const _data = ref<any>(clone(props.data));

async function submit($event: SubmitEventPromise) {
   const validationResults = await $event;
   if (validationResults.valid) {
      dialog.value = false;
      emit("save", _data);
   }
}
onBeforeUpdate(() => {
   // reset the values of ssid and password when the dialog is closed
   _data.value = clone(props.data);
});
</script>
