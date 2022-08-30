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
1. Klonen des Projekts von [GitHub](https://github.com/ynctlm/cas-iot-projekt)
   ```
   git clone https://github.com/ynctlm/cas-iot-projekt.git
   ```
2. Anpassen der Eigenschaft `spec.hostPath.path` in allen Dateien im Ordner `k8s`. Es muss jeweils der Pfad des heruntergeladenen Git-Repositories angegeben werden.
3. Starten des Kubernetes-Clusters
   ```
   kubectl apply -f k8s
   ```
4. Den Start der Container mit folgendem Befehl überwachen
   ```
   watch kubectl get all
   ```
5. Wenn die Pods `grafana`, `mariadb` und `nodered` im Status `Running` sind, können (Node-RED)[localhost:1880] und (Grafana)[localhost:3000] im Browser geöffnet werden.
6. Die Grafana-Credentials sind:
   
   User: admin
   
   Passwort: admin

## Initialisieren der Datenbank mit Node-RED Flow
1. Hierzu (Node-RED)[localhost:1880] ca. 60 aufrufen, nachdem die Kubernetes-Services erfolgreich gestartet wurden. 
2. Dann eine beliebige Node in Node-RED verschieben und per `Übernahme (deploy)` bestätigen. 
3. Im Anschluss wird automatisiert die Datenbank aufgebaut (der Erfolg lässt sich im Debug-Output von Node-RED nachvollziehen).
4. Im Tab `Demo: Insert test-data` in Node-RED lassen sich Demodaten zur Datenbank hinzufügen, um die Grafana-Dashboards zu testen.


# Uninstall
Delete everything with:
```
kubectl delete service mariadb
kubectl delete deployment mariadb
kubectl delete pvc mariadb-pv-claim
kubectl delete pv mariadb-pv-volume

kubectl delete service grafana
kubectl delete deployment grafana
kubectl delete pvc grafana-pv-claim
kubectl delete pv grafana-pv-volume

kubectl delete service nodered
kubectl delete deployment nodered
kubectl delete pvc nodered-pv-claim
kubectl delete pv nodered-pv-volume
```