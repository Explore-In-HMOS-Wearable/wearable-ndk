export interface RenderContext {
  updateTransformMatrix(eventType: number, mXAngle: number, mYAngle: number): void
}

/**
 * Adds a metaball at the specified screen coordinates
 * @param context - XComponent context
 * @param x - X coordinate on screen
 * @param y - Y coordinate on screen
 */
export const addMetaball: (context: ESObject, x: number, y: number) => void;

/**
 * Clears all metaballs from the scene
 * @param context - XComponent context
 */
export const clearMetaballs: (context: ESObject) => void;

export const getContext: (value: number) => ESObject;