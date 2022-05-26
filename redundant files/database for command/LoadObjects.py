import boto3
from boto3.dynamodb.conditions import Key
import datetime


def query_objects(Object, dynamodb=None):
	if not dynamodb:
		dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

	table = dynamodb.Table('Objects')

	# Expression attribute names can only reference items in the projection expression.
	response = table.query(KeyConditionExpression=Key('Object').eq(Object), ScanIndexForward=False)
	return response['Items']

def get_objects ():
	objects_resp = []
	print(f"All objects:")
	for i in range(0, len(query_objects)):
		objects_resp.append(query_objects(i)) # get all objects from the table
    

if __name__ == "__main__":
	get_objects()