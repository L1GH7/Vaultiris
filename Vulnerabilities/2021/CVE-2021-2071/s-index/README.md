# CVE-2021-20717-EC-CUBE-XSS

## NVD Description

Cross-site scripting vulnerability in EC-CUBE 4.0.0 to 4.0.5 allows a remote attacker to inject a specially crafted script in the specific input field of the EC web site which is created using EC-CUBE. As a result, it may lead to an arbitrary script execution on the administrator's web browser.

## Set Up

1. Clone EC Cube

```
git clone https://github.com/EC-CUBE/ec-cube.git -b 4.0.5
```

2. Docker Build

```
docker build -t eccube4-php-apache .
```

3. Docker Run

```
docker run --name ec-cube -p "8080:80" -p "4430:443" eccube4-php-apache
```

## Reproduce

### User

http://localhost:8080

1. Add to Cart & Proceed to Checkout

2. Inject Script to Inquiry Form

![Inject Script to Inquiry Form](https://user-images.githubusercontent.com/56715563/120063116-8e527100-c0a0-11eb-8ba9-aff86d8a4ca1.png)

3. Complete Purchase

### Admin

http://localhost:8080/admin/login

1. Login admin:password

2. Open Order List Page

3. Click (Malicious) Order

4. Click Create Mail Button

5. Select Template

6. Click Confirm Mail Button

![Click Confirm Mail Button](https://user-images.githubusercontent.com/56715563/120063179-e8533680-c0a0-11eb-8479-18fb43db7f1e.png)

## Fixed Commit

https://github.com/EC-CUBE/ec-cube/commit/9bae20f070acfe0b84451bc1e082c699ac197faf

