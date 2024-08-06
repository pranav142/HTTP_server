import requests

def main():
    url = 'http://localhost:4221'

    try:
        response = requests.get(url)
        
        if response.status_code == 200:
            print("Response from server:", response.text)
        else:
            print("Failed to connect to server, status code:", response.status_code)
    except requests.exceptions.RequestException as e:
        print("Error communicating with the server:", e)

if __name__ == "__main__":
    main()

