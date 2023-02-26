import axios from "axios";
import * as t from "io-ts";
const apiBaseUrl = import.meta.env.VITE_APP_API_BASE_URL;

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
   angle: t.number,
   speed: t.number,
   pin: t.union([t.number, t.undefined]),
   address: t.union([t.number, t.undefined]),
});

export type Led = t.TypeOf<typeof ledCodec>;

const statusCodec = t.type({
   leds: t.array(ledCodec),
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
   async switchLed(id: number, isOn: boolean) {
      try {
         await axiosInstance.post("/api/led", { id, isOn });
      } catch (error) {
         console.error(error);
         throw error;
      }
   },
};
