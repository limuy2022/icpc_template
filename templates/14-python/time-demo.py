from datetime import datetime, timedelta

fmt = "%Y-%m-%d %H:%M:%S"
start = datetime.strptime("2024-12-31 23:45:00", fmt)
end = start + timedelta(days=1, minutes=30)
print(end.strftime(fmt))
delta = end - start
print(delta.days * 86400 + delta.seconds)

day_offset, minute = divmod(23 * 60 + 45 + 30, 1440)
hour, minute = divmod(minute, 60)
print(day_offset, f"{hour:02d}:{minute:02d}")
