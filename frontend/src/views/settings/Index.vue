<script lang="ts" setup>
import ViewWrapper from "@/components/ViewWrapper.vue";
import WifiCredentials from "./WifiCredentials.vue";
import Servos from "./Servos.vue";
import LedsCard from "./LedsCard.vue";
</script>
<template>
   <ViewWrapper>
      <v-row no-gutters justify="center" cols="12">
         <v-col xs="12" md="10" lg="8" xl="6" class="pa-2"
            ><WifiCredentials />
         </v-col>
      </v-row>
      <v-row no-gutters justify="center" cols="12">
         <v-col xs="12" md="10" lg="8" xl="6" class="pa-2"><Servos /> </v-col>
      </v-row>

      <v-row no-gutters justify="center" cols="12">
         <v-col xs="12" md="10" lg="8" xl="6" class="pa-2"><LedsCard /> </v-col>
      </v-row>
   </ViewWrapper>
</template>

<style scoped></style>
