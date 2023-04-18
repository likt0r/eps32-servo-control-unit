export function isRequired(fieldName: string) {
   return (value: any) => {
      console.log('value', value)
      return (typeof value !== 'undefined' && value !== null && value !== '' ? true : `${fieldName} is required`)
   };
}
export function isGreaterEqualValue(fieldName: string, value: number) {
   return (fieldValue: number) => fieldValue >= value || `${fieldName} must be greater than or equal to 0`;
}

export function isGreaterOtherFieldValue(fieldName: string, otherFieldName: string, otherFieldValue: number) {
   return (value: number) => value > otherFieldValue || `${fieldName} must be greater than ${otherFieldName}`;
}

export function isSmallerEqualValue(fieldName: string, value: number) {
   return (fieldValue: number) => fieldValue < value || `${fieldName} must be smaller than ${value}`;
}