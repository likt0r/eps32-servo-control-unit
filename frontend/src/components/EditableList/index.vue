<template>
   <v-list>
      <v-list-item
         v-for="(item, index) in list"
         :key="idFunction(item)"
         :title="titleFunction(item)"
         :subtitle="subtitleFunction(item)"
      >
         <template v-slot:append>
            <EditDialog
               :persistent="true"
               :title="`Edit ${titleFunction(item)}`"
               :form="formComponent"
               :button-icon="mdiPencil"
               :data="item"
               :form-component="formComponent"
               @save="(item) => onEdit(index, item)"
               :disabled="disabled"
            ></EditDialog>
            <v-btn
               :icon="mdiDelete"
               variant="text"
               color="secondary"
               @click="onDelete(index)"
               :disabled="disabled"
            ></v-btn>
         </template>
      </v-list-item>

      <v-list-item>
         <template v-slot:append>
            <EditDialog
               :persistent="true"
               :title="`Add new Item}`"
               :form="formComponent"
               :button-icon="mdiPlusThick"
               :form-component="formComponent"
               @save="(item) => onAdd(item)"
               :data="defaultData"
               :disabled="disabled"
            ></EditDialog>
         </template>
      </v-list-item>
   </v-list>
</template>
<script setup lang="ts">
import { defineProps, ref, computed, withDefaults, PropType } from "vue";
import { mdiDelete, mdiPlusThick, mdiPencil } from "@mdi/js";
import clone from "lodash/cloneDeep";
import EditDialog from "./EditDialog.vue";

const props = defineProps({
   idFunction: {
      type: Function as PropType<(item: any) => string>,
      required: true,
   },
   titleFunction: {
      type: Function as PropType<(item: any) => string>,
      required: true,
   },
   subtitleFunction: {
      type: Function as PropType<(item: any) => string>,

      required: true,
   },
   modelValue: {
      type: Array as PropType<any[]>,
      required: true,
   },
   defaultData: {
      type: Object as PropType<any>,
      required: true,
   },
   formComponent: {
      type: Object as PropType<any>,
      required: true,
   },
   optimistic: {
      type: Boolean as PropType<boolean>,
      default: false,
      required: false,
   },
   disabled: {
      type: Boolean as PropType<boolean>,
      default: false,
      required: false,
   },
});
const newItem = ref<any>(clone(props.defaultData));
const emit = defineEmits<{
   (e: "update:modelValue", value: any[]): void;
   (
      e: "changed",
      type: "add" | "edit" | "delete",
      oldValue: any[] | null,
      newValue: any[] | null
   ): void;
}>();
const list = computed({
   get() {
      return props.modelValue;
   },
   set(value) {
      emit("update:modelValue", value);
   },
});

function onDelete(index: number) {
   const removedItem = list.value[index];
   list.value.splice(index, 1);
   emit("changed", "delete", removedItem, null);
}

function onAdd(item: any) {
   list.value.push(item.value);
   newItem.value = clone(props.defaultData);
   console.log(item);
   emit("changed", "add", null, list.value);
}

function onEdit(index: number, item: any) {
   const oldList = clone(list.value);
   list.value[index] = item.value;
   emit("changed", "edit", oldList, list.value);
}
</script>
<style scoped></style>
