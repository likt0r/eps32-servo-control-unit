export function isRequired(fieldName: string) {
   return (value: any) => (value ? true : `${fieldName} is required`);
}
