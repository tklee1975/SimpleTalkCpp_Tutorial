using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyChar : MonoBehaviour
{
    public int bulletCount = 20;
    public int bullectCountMax = 20;

    MyPortal lastPortal;

    public float speed = 5; // code or Game Designer
    public float rotateSpeed = 180;

    public float fireCooldown = 0.5f;
    float fireCooldownRemain = 0;

    public GameObject fireLocation;

    public MyBullet bulletPrefab;

    [SerializeField]
    float m_testingAbc = 1; // changed by Game Designer
    public float testingAbc => m_testingAbc;

    float _testing2 = 1;
    public float testing2 {  // changed by code only
        get { return _testing2; }
        set { _testing2 = value; }
    }

    // Start is called before the first frame update
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        if (fireCooldownRemain > 0) {
            fireCooldownRemain -= Time.deltaTime;
        }

        float moveDir = 0;
        if (Input.GetKey(KeyCode.W)) {
            moveDir += 1;
        } else if (Input.GetKey(KeyCode.S)) {
            moveDir -= 1;
        }

        Vector3 pos = transform.position;
        pos += transform.forward * moveDir * speed * Time.deltaTime;
        transform.position = pos;

        float r = 0;
        if (Input.GetKey(KeyCode.A)) {
            r -= 1;
        } else if (Input.GetKey(KeyCode.D)) {
            r += 1;
        }

        Quaternion rot = transform.rotation;
        Vector3 e = rot.eulerAngles;
        e.y += r * rotateSpeed * Time.deltaTime;
        transform.rotation = Quaternion.Euler(e);

        if (Input.GetKey(KeyCode.J)) {
            Fire();
        }
    }

	private void OnGUI()
	{
        GUILayout.Space(100);
        GUILayout.BeginHorizontal();
        {
            GUILayout.Space(20);
            GUILayout.Box($"Bullet Count: {bulletCount} / {bullectCountMax}");
            GUILayout.Box($"Last Portal: {(lastPortal ? lastPortal.name : "")}");


		    if (GUILayout.Button("Reload")) {
                Debug.Log("Reload");
                Debug.DrawLine(transform.position, transform.position + new Vector3(0, 10, 0), new Color(0,1,0), 3);
                bulletCount = 6;
            }
        }
        GUILayout.EndHorizontal();
	}

	private void OnDrawGizmosSelected()
	{
        Gizmos.matrix = transform.localToWorldMatrix;
        Gizmos.color = new Color(1,0,0, 0.8f);
		//Gizmos.DrawCube(new Vector3(0, 3, 0), new Vector3(2,2,1));
  //      Gizmos.DrawLine(Vector3.zero, new Vector3(0,0, 5));
	}

	void Fire() {
        if (bulletCount <= 0) return;
        if (fireCooldownRemain > 0) return;

        if (bulletPrefab == null) {
            Debug.LogError("Missing Bullet Prefab");
            return;
        }

        bulletCount--;
        fireCooldownRemain = fireCooldown;
        var obj = Instantiate(bulletPrefab.gameObject);
        obj.transform.position = fireLocation ? fireLocation.transform.position : transform.position;
        obj.transform.rotation = transform.rotation;
//        MyBullet.Create(transform);
    }

    void ReloadFromPoint(MyReloadPoint p) {
        var n = bullectCountMax - bulletCount;
        if (p.bulletCount < n) n = p.bulletCount;
        if (n <= 0) return;

        bulletCount += n;
        p.bulletCount -= n;
        // Debug.Log("Bullet +" + n);
    }

    void Teleport(MyPortal p) {
        if (p == lastPortal) return;

        var t = p.target;
        if (!t) return;

        lastPortal = t;
        transform.position = t.transform.position;
    }

	private void OnTriggerEnter(Collider other)
	{
        // Debug.Log("Trigger Enter");
        {
            var p = other.GetComponent<MyReloadPoint>();
            if (p) ReloadFromPoint(p); 
        }

        {
            var p = other.GetComponent<MyPortal>();
            if (p) Teleport(p);
        }
	}

	private void OnTriggerExit(Collider other)
	{
        {
            var p = other.GetComponent<MyPortal>();
            if (p == lastPortal) {
                lastPortal = null;
            }
        }
	}
}
