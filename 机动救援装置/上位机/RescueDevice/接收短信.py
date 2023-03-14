import smtplib
from email.mime.text import MIMEText
from email.utils import formataddr


sender = '1390540209@qq.com'
receiver = ['1390540209@qq.com']

# 三个参数：第一个为文本内容，第二个 plain 设置文本格式，第三个 utf-8 设置编码
message = MIMEText('机动救援装置预警', 'plain', 'utf-8')

message['From'] = formataddr(["ParaTu", sender])  # 发送者
message['To'] = formataddr(["警报接收者", receiver[0]])  # 接收者

message['Subject'] = '您的设备出现紧急情况，请及时处理\n' \
                     '必要时请拨打紧急救援电话！！！'

try:
        smtpObj = smtplib.SMTP()
        smtpObj.connect("smtp.qq.com", 587)
        smtpObj.login(user='1390540209@qq.com', password='qnalvcrhzygeigfe')
        smtpObj.sendmail(sender, receiver, message.as_string())

except smtplib.SMTPException:
        print("Error: unable to send email")
else:
        print("Successfully sent email")
        smtpObj.quit()




