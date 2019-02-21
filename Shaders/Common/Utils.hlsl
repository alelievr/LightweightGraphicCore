// Return the direction vector of the camera using pixel coordinates between 0 and 1
float3 GetViewDirection(float2 cameraUV, float4x4 inverseCameraView)
{
    float4 clipDirection = float4(cameraUV * 2.0 - 1.0, 1, 0);

    float4 eyeDirection = camera.projection * clipDirection;
    eyeDirection.zw = float2(1, 0);
    return normalize(inverseCameraView * eyeDirection);
}