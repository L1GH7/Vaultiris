FROM python:3
COPY app/ .
COPY requirements.txt .
COPY flag.txt .
RUN pip install -r requirements.txt
CMD python app.py
