import boto3
def delete_objects_table(dynamodb=None):
 if not dynamodb:
    dynamodb = boto3.resource('dynamodb', region_name="us-east-1")
    table = dynamodb.Table('Objects')
    table.delete()
if __name__ == '__main__':
 delete_objects_table()
 print("Objects table deleted.")
