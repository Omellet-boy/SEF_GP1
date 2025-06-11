# send_email.py
import sys
import smtplib
from email.mime.text import MIMEText

# Args: email_address, subject, body
if len(sys.argv) < 4:
    sys.exit("Usage: send_email.py email subject body")

email = sys.argv[1]
subject = sys.argv[2]
body = sys.argv[3]

sender_email = "notisystempy@gmail.com"
app_password = "cffzspuubpxveoge"  # Replace with your 16-character App Password from Google

msg = MIMEText(body)
msg["Subject"] = subject
msg["From"] = sender_email
msg["To"] = email

try:
    with smtplib.SMTP("smtp.gmail.com", 587) as server:
        server.starttls()
        server.login(sender_email, app_password)
        server.send_message(msg)
    print("Email sent successfully.")
except Exception as e:
    print("Error:", e)
    sys.exit(1)
