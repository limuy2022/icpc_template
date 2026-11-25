from calendar import isleap, monthrange
from datetime import date, timedelta


def add_months(day, months):
    year, month = divmod(day.year * 12 + day.month - 1 + months, 12)
    month += 1
    return date(year, month, min(day.day, monthrange(year, month)[1]))


start = date.fromisoformat("2024-01-31")
end = date(2024, 3, 1)
print(start + timedelta(days=29), (end - start).days)
print(isleap(2024), monthrange(2024, 2)[1])
print(add_months(start, 1))
offset = (0 - start.weekday()) % 7
print(start + timedelta(days=offset or 7))
print(date.fromordinal(start.toordinal() + 1))
