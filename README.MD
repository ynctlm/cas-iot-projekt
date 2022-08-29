# Kubernetes
## Konzeption
Drei Services werden benötigt:
| Service  | Beschreibung                                                                                                              |
| -------- | ------------------------------------------------------------------------------------------------------------------------- |
| Node-RED | Node-RED ist der zentrale Service des IoT-Use-Cases und wird bspw. für die Kommunikation mit dem ESP32 über MQTT benötigt |
| Grafana  | Grafana dient als Dashboard für die Visualisierung der vom ESP32 gesendeten Daten                                         |
| MariaDB  | MariaDB ist die zugrunde liegende Datenbank, die die Daten für die Visualisierung in Grafana speichert                    |

## Voraussetzungen
- Kubernetes-Installation (kubectl wird benötigt)
- Git-Installation

## Aufbau des Kubernetes-Clusters
1. Klonen des Projekts von (GitHub)[]
   ```
   git clone https://github.com/ynctlm/cas-iot-projekt.git
   ```
2. Starten des Kubernetes-Clusters
   ```
   kubectl apply -f k8s/nodered.yaml
   kubectl apply -f k8s/grafana.yaml
   kubectl apply -f k8s/mariadb.yaml
   ```
3. Installation von 



# Uninstall
Delete everything with:
```
kubectl delete service mariadb
kubectl delete deployment mariadb
kubectl delete pvc mariadb-pv-claim
kubectl delete pv mariadb-pv-volume
rm -r mariadb

kubectl delete service grafana
kubectl delete deployment grafana
kubectl delete pvc grafana-pv-claim
kubectl delete pv grafana-pv-volume
rm -r grafana

kubectl delete service nodered
kubectl delete deployment nodered
kubectl delete pvc nodered-pv-claim
kubectl delete pv nodered-pv-volume
rm -r nodered
```