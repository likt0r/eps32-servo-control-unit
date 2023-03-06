import axios from "axios";
import * as t from "io-ts";
const apiBaseUrl = import.meta.env.VITE_APP_API_BASE_URL;
import { fromEnum } from "./tools";
console.log(apiBaseUrl);
const axiosInstance = axios.create({
   baseURL: apiBaseUrl,
});

const ledCodec = t.type({
   id: t.number,
   isOn: t.boolean,
   pin: t.number,
});

const servoCodec = t.type({
   id: t.number,
   pin: t.number,
   position: t.number,
   minPwm: t.number,
   maxPwm: t.number,
   minAngle: t.number,
   maxAngle: t.number,
});
export enum MotionMode {
   Idle = "idle",
   Startup = "startup",
   Remote_Control = "remote_control",
   Editor = "editor",
}
export const motionModeCodec = fromEnum<MotionMode>("MotionMode", MotionMode);

export const KeyframeCodec = t.type({
   name: t.string,
   id: t.string,
   transitionDuration: t.number,
   pauseDuration: t.number,
   targetPositions: t.array(
      t.type({
         id: t.number,
         position: t.number,
      })
   ),
});

export const MotionSequenceCodec = t.type({
   name: t.string,
   id: t.string,
   keyframes: t.array(KeyframeCodec),
   loop: t.boolean,
   speedMultiplier: t.number,
});

export type Keyframe = t.TypeOf<typeof KeyframeCodec>;
export type MotionSequence = t.TypeOf<typeof MotionSequenceCodec>;

export type Led = t.TypeOf<typeof ledCodec>;
export type Servo = t.TypeOf<typeof servoCodec>;

const statusCodec = t.type({
   leds: t.array(ledCodec),
   servos: t.array(servoCodec),
});
export type Status = t.TypeOf<typeof statusCodec>;

export default {
   async getLeds(): Promise<Led[]> {
      try {
         const response = await axiosInstance.get<Status>("/api/status");
         const result = statusCodec.decode(response.data);
         // type guard
         if (result._tag === "Left") {
            throw new Error(
               `Response data has an unexpected type: ${result.left}`
            );
         }
         return result.right.leds;
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async getServos(): Promise<Servo[]> {
      try {
         const response = await axiosInstance.get<Status>("/api/status");
         if (response.status !== 200) {
            throw new Error(
               `Unexpected status code: ${response.status} ${response.statusText}`
            );
         }
         console.log(response.data);
         const result = statusCodec.decode(response.data);
         // type guard
         if (result._tag === "Left") {
            console.log(JSON.stringify(result.left));
            throw new Error(
               `Response data has an unexpected type: ${JSON.stringify(
                  result.left
               )}`
            );
         }
         return result.right.servos;
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async switchLed(id: number, isOn: boolean) {
      try {
         await axiosInstance.post("/api/led", { id, isOn });
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async setServoPosition(id: number, position: number) {
      try {
         await axiosInstance.post("/api/motion/position", {
            id,
            position,
         });
      } catch (error) {
         console.error(error);
         throw error;
      }
   },

   async setMotionSpeed(speed: number) {
      try {
         await axiosInstance.post("/api/motion/speed", { speed });
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async getMotionSpeed(): Promise<number> {
      try {
         const result = await axiosInstance.get("/api/motion/speed");
         return result.data;
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async setMotionMode(mode: MotionMode) {
      try {
         await axiosInstance.post("/api/motion/mode", { mode });
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
   async getMotionMode(): Promise<MotionMode> {
      try {
         const response = await axiosInstance.get("/api/motion/mode");
         const result = motionModeCodec.decode(response.data);
         // type guard
         if (result._tag === "Left") {
            throw new Error(
               `Response data has an unexpected type: ${result.left}`
            );
         }
         return result.right;
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
};
