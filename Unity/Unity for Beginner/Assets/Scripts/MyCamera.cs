using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyCamera : MonoBehaviour
{
    public Transform aim;
    public Camera _camera;
	public float cameraAngleY = 30;

	public float cameraAngleX = 45;
	public float cameraAngleXMin = 5;
	public float cameraAngleXMax = 85;

	public float cameraDistance = 10;
	public float cameraDistanceMin = 5;
	public float cameraDistanceMax = 50;

	public float mouseFactor = 1;
	public float mouseWheelFactor = 1;

	public float dampingTime = 0.5f;

	Vector3 _currentVelocity;

	Vector3 _lastMousePos;

	private void OnDrawGizmos()
	{
		if (_camera) {
			Gizmos.DrawLine(transform.position, _camera.transform.position);
		}
	}

	void Update()
    {
		if (Input.GetMouseButton(1)) {
			var deltaPos = Input.mousePosition - _lastMousePos;

//			transform.Rotate(0,	deltaPos.x * mouseFactor, 0);
			cameraAngleY += deltaPos.x * mouseFactor;
			cameraAngleX += deltaPos.y * mouseFactor;
		}

		cameraAngleX = Mathf.Clamp(cameraAngleX, cameraAngleXMin, cameraAngleXMax);

		cameraDistance -= Input.mouseScrollDelta.y * mouseWheelFactor;
		cameraDistance = Mathf.Clamp(cameraDistance, cameraDistanceMin, cameraDistanceMax);

        if (aim) {
			transform.position = Vector3.SmoothDamp(transform.position, aim.position, ref _currentVelocity, dampingTime);
		}

		if (_camera) {
			var q = Quaternion.AngleAxis(cameraAngleY, Vector3.up)
				  * Quaternion.AngleAxis(-cameraAngleX, Vector3.right);
			var v = q * Vector3.forward;

			_camera.transform.localPosition = v * cameraDistance;
			_camera.transform.LookAt(transform);
		}

		_lastMousePos = Input.mousePosition;
    }
}
