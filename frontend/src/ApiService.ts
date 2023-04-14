import { DefaultApi, Configuration } from "./generated/api";
export * from './generated/api';
const apiBaseUrl = import.meta.env.VITE_APP_API_BASE_URL as string;

export const apiService = new DefaultApi(new Configuration({ basePath: apiBaseUrl }))
export default apiService;