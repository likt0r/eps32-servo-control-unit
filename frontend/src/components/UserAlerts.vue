<template>
   <v-container transition="scale-transition">
      <transition-group name="alert-transition">
         <div
            class="alert-background"
            v-for="alertContent in alertContents"
            :key="alertContent.id"
         >
            <v-alert
               border="start"
               variant="tonal"
               closable
               close-label="Close Alert"
               :type="alertContent.type"
               :title="alertContent.title"
               :text="alertContent.text"
               ><template v-slot:close="{}">
                  <v-btn
                     :icon="mdiClose"
                     @click="close(alertContent.id)"
                  ></v-btn> </template
            ></v-alert>
         </div>
      </transition-group>
   </v-container>
</template>
<style scoped>
.v-container {
   padding: 0;
   max-width: 600px;
   position: fixed;
   top: 0;
   right: 0;
   z-index: 9999;
}
.alert-background {
   background-color: white;
   margin: 0.5rem;
}
.alert-transition-enter-active,
.alert-transition-leave-active {
   transition: opacity 0.5s;
}
.alert-transition-enter,
.alert-transition-leave-to {
   opacity: 0;
}
</style>
<script setup lang="ts">
import { ref } from "vue";
import { storeToRefs } from "pinia";
import { mdiClose } from "@mdi/js";
import useAppStore from "@/store/app";

const { alertContents } = storeToRefs(useAppStore());
const { unpublishAlert } = useAppStore();
function close(id: string) {
   unpublishAlert(id);
}
</script>
